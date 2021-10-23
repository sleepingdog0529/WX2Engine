#include "keyboard.h"
#include "input.h"

namespace wx2
{
	Keyboard::Keyboard() :
		directInput_(),
		hwnd_()
	{
	}

	Keyboard::~Keyboard()
	{
		for (auto& kd : keyboards_)
		{
			if (kd.device)
			{
				kd.device->Unacquire();
			}
		}
	}

	void Keyboard::Initialize(DInputPtr directInput, HWND hwnd)
	{
		WX2_ASSERT_MSG(directInput, "IDirectInputDevice8がnullptrです。");
		WX2_ASSERT_MSG(IsWindowEnabled(hwnd) != 0, "ウィンドウハンドルが無効です。");
		directInput_ = directInput;
		hwnd_ = hwnd;
	}

	void Keyboard::Regist()
	{
		HRESULT hr;

		hr = directInput_->EnumDevices(
			DI8DEVTYPE_KEYBOARD,
			SetupKeyboardCallback,
			this,
			DIEDFL_ATTACHEDONLY);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイスティックデバイスの作成に失敗しました。");
			exit(EXIT_FAILURE);
		}
	}

	void Keyboard::Update()
	{
		HRESULT hr;

		state_.previous = std::move(state_.current);
		std::memset(&state_.current, 0, sizeof(state_.current));

		BYTE stateBuffer[NUM_KEYS_] = {};

		// 全てのキーボードの状態を取得
		for (auto& kd : keyboards_)
		{
			hr = kd.device->Poll();
			if (FAILED(hr))
			{
				// 切断されていた場合再登録を試みる
				kd.device->Acquire();
				continue;
			}

			std::memset(&stateBuffer, 0, sizeof(stateBuffer));

			// キーボード状態取得
			hr = kd.device->GetDeviceState(static_cast<DWORD>(sizeof(stateBuffer)), &stateBuffer);
			if (FAILED(hr))
			{
				WX2_LOG_WARN("キーボード状態取得に失敗しました。");
				continue;
			}

			// キー報を格納
			for (std::size_t i = 0; i < NUM_KEYS_; ++i)
			{
				state_.current.keys[i] = state_.current.keys[i] | (stateBuffer[i] & 0x80);
			}
		}
	}

	bool Keyboard::IsDown(Keys key) const
	{
		return state_.current.keys[key];
	}

	bool Keyboard::IsPressed(Keys key) const
	{
		return state_.current.keys[key] & !state_.previous.keys[key];
	}

	bool Keyboard::IsReleased(Keys key) const
	{
		return !state_.current.keys[key] & state_.previous.keys[key];
	}

	BOOL Keyboard::SetupKeyboardCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
	{
		WX2_ASSERT_MSG(
			GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_KEYBOARD,
			"マウス以外のデバイスは列挙できません");

		auto* keyboard = reinterpret_cast<Keyboard*>(pvRef);
		WX2_ASSERT_MSG(keyboard, "Keyboardクラスがnullptrです。");

		// デバイス作成済みの場合スキップする
		if (std::ranges::any_of(keyboard->keyboards_,
			[&lpddi](const KeyboardDevice& md) { return md.instance.guidInstance == lpddi->guidInstance; }))
		{
			return DIENUM_CONTINUE;
		}

		HRESULT hr;
		KeyboardDevice keyboardDev = {};
		keyboardDev.instance = std::move(static_cast<DIDEVICEINSTANCE>(*lpddi));

		// デバイス作成
		hr = keyboard->directInput_->CreateDevice(
			keyboardDev.instance.guidInstance,
			keyboardDev.device.GetAddressOf(),
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスの作成に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// 機能情報を取得
		keyboardDev.capability.dwSize = static_cast<DWORD>(sizeof(keyboardDev.capability));
		hr = keyboardDev.device->GetCapabilities(&keyboardDev.capability);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスの機能情報取得に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// データフォーマット設定
		hr = keyboardDev.device->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスのフォーマットの設定に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// 協調モード設定
		hr = keyboardDev.device->SetCooperativeLevel(keyboard->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("キーボードデバイスの協調モード設定に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				keyboardDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(keyboardDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		keyboard->keyboards_.push_back(std::move(keyboardDev));

		return DIENUM_CONTINUE;
	}
}