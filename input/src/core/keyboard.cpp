#include "keyboard.h"
#include "input.h"

namespace wx2
{
	Keyboard::~Keyboard() noexcept
	{
		// 登録された全てのキーボードデバイスを解放
		for (auto& [device, instance, capability] : keyboards_)
		{
			if (device)
			{
				device->Unacquire();
			}
		}
	}

	void Keyboard::Initialize(const DInputPtr& directInput, const HWND hwnd) noexcept
	{
		WX2_ASSERT_MSG(directInput, "IDirectInputDevice8がnullptrです。");
		WX2_ASSERT_MSG(IsWindowEnabled(hwnd) != 0, "ウィンドウハンドルが無効です。");

		directInput_ = directInput;
		hwnd_ = hwnd;
	}

	void Keyboard::Regist()
	{
		// 現在接続中のデバイスを列挙して登録
		const HRESULT hr = directInput_->EnumDevices(
			DI8DEVTYPE_KEYBOARD,
			SetupKeyboardCallback,
			this,
			DIEDFL_ATTACHEDONLY);
		WX2_COM_ERROR_IF_FAILED(hr, "キーボードデバイスの作成に失敗しました。");
	}

	void Keyboard::Update() noexcept
	{
		// 前フレームの情報を更新
		state_.previous = state_.current;
		state_.current = {};

		// 全てのキーボードの状態を取得
		for (const auto& [device, instance, capability] : keyboards_)
		{
			HRESULT hr = device->Poll();
			if (FAILED(hr))
			{
				// 切断されていた場合再登録を試みる
				device->Acquire();
				continue;
			}

			// キーボード状態取得
			hr = device->GetDeviceState(static_cast<DWORD>(sizeof(stateBuffer_)), &stateBuffer_);
			if (FAILED(hr))
			{
				WX2_LOG_WARN("キーボード状態取得に失敗しました。");
				continue;
			}

			// キー入力情報を格納
			for (std::size_t i = 0; i < NUM_KEYS; ++i)
			{
				state_.current.keys[i] = state_.current.keys[i] || (stateBuffer_[i] & 0x80);
			}
		}
	}

	BOOL Keyboard::SetupKeyboardCallback(LPCDIDEVICEINSTANCE lpddi, const LPVOID pvRef) noexcept
	{
		WX2_ASSERT_MSG(
			GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_KEYBOARD,
			"マウス以外のデバイスは列挙できません");

		auto* keyboard = static_cast<Keyboard*>(pvRef);
		WX2_ASSERT_MSG(keyboard, "Keyboardクラスがnullptrです。");

		// デバイス作成済みの場合スキップする
		if (std::ranges::any_of(keyboard->keyboards_,
			[&lpddi](const KeyboardDevice& md) { return md.instance.guidInstance == lpddi->guidInstance; }))
		{
			return DIENUM_CONTINUE;
		}

		// キーボードのデバイス情報
		KeyboardDevice keyboardDev = {};
		keyboardDev.instance = static_cast<DIDEVICEINSTANCE>(*lpddi);

		// キーボードのデバイス作成
		HRESULT hr = keyboard->directInput_->CreateDevice(
			keyboardDev.instance.guidInstance,
			keyboardDev.device.GetAddressOf(),
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスの作成に失敗しました。製品名: {} 種別: {:X}",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// 機能情報を取得
		keyboardDev.capability.dwSize = static_cast<DWORD>(sizeof(keyboardDev.capability));
		hr = keyboardDev.device->GetCapabilities(&keyboardDev.capability);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスの機能情報取得に失敗しました。製品名: {} 種別: {:X}",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// データフォーマット設定
		hr = keyboardDev.device->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスのフォーマットの設定に失敗しました。製品名: {} 種別: {:X}",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// 協調モード設定
		hr = keyboardDev.device->SetCooperativeLevel(keyboard->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスの協調モード設定に失敗しました。製品名: {} 種別: {:X}",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// 登録済みキーボードデバイスリストに追加
		keyboard->keyboards_.push_back(std::move(keyboardDev));

		return DIENUM_CONTINUE;
	}
}