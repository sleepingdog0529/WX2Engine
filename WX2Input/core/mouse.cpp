#include "mouse.h"
#include "input.h"

namespace wx2
{
	Mouse::Mouse() :
		directInput_(),
		hwnd_()
	{
	}

	Mouse::~Mouse()
	{
		for (auto& md : mouses_)
		{
			if (md.device)
			{
				md.device->Unacquire();
			}
		}
	}

	void Mouse::Initialize(DInputPtr directInput, HWND hwnd)
	{
		WX2_ASSERT_MSG(directInput, "IDirectInputDevice8がnullptrです。");
		WX2_ASSERT_MSG(IsWindowEnabled(hwnd) != 0, "ウィンドウハンドルが無効です。");
		directInput_ = directInput;
		hwnd_ = hwnd;
	}

	void Mouse::Regist()
	{
		HRESULT hr;

		hr = directInput_->EnumDevices(
			DI8DEVTYPE_MOUSE,
			SetupMouseCallback,
			this,
			DIEDFL_ATTACHEDONLY);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイスティックデバイスの作成に失敗しました。");
			exit(EXIT_FAILURE);
		}
	}

	void Mouse::Update()
	{
		HRESULT hr;

		state_.previous = std::move(state_.current);
		std::memset(&state_.current, 0, sizeof(state_.current));
		std::memset(&state_.axises, 0, sizeof(state_.axises));

		DIMOUSESTATE2 stateBuffer = {};

		// 全てのマウスの状態を取得
		for (auto& md : mouses_)
		{
			hr = md.device->Poll();
			if (FAILED(hr))
			{
				// 切断されていた場合再登録を試みる
				md.device->Acquire();
				continue;
			}

			std::memset(&stateBuffer, 0, sizeof(stateBuffer));
			
			// マウス状態取得
			hr = md.device->GetDeviceState(static_cast<DWORD>(sizeof(stateBuffer)), &stateBuffer);
			if (FAILED(hr))
			{
				WX2_LOG_WARN("マウス状態取得に失敗しました。");
				continue;
			}

			// ボタン情報を格納
			for (std::size_t i = 0; i < NUM_BUTTONS_; ++i)
			{
				state_.current.buttons[i] = state_.current.buttons[i] | (stateBuffer.rgbButtons[i] & 0x80);
			}

			// 軸情報を格納
			state_.axises[CursorX] += static_cast<float>(stateBuffer.lX);
			state_.axises[CursorY] += static_cast<float>(stateBuffer.lY);
			state_.axises[WheellScroll] += stateBuffer.lZ / (float)WHEEL_DELTA;
		}
	}

	bool Mouse::IsDown(Buttons button) const
	{
		return state_.current.buttons[button];
	}

	bool Mouse::IsPressed(Buttons button) const
	{
		return state_.current.buttons[button] & !state_.previous.buttons[button];
	}

	bool Mouse::IsReleased(Buttons button) const
	{
		return !state_.current.buttons[button] & state_.previous.buttons[button];
	}

	float Mouse::GetAxisVelocity(Axises axises) const
	{
		return state_.axises[axises];
	}

	BOOL Mouse::SetupMouseCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
	{
		WX2_ASSERT_MSG(
			GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_MOUSE,
			"マウス以外のデバイスは列挙できません");

		auto* mouse = reinterpret_cast<Mouse*>(pvRef);
		WX2_ASSERT_MSG(mouse, "Mouseクラスがnullptrです。");

		// デバイス作成済みの場合スキップする
		if (std::ranges::any_of(mouse->mouses_,
			[&lpddi](const MouseDevice& md) { return md.instance.guidInstance == lpddi->guidInstance; }))
		{
			return DIENUM_CONTINUE;
		}

		HRESULT hr;
		MouseDevice mouseDev = {};
		mouseDev.instance = std::move(static_cast<DIDEVICEINSTANCE>(*lpddi));

		// デバイス作成
		hr = mouse->directInput_->CreateDevice(
			mouseDev.instance.guidInstance,
			mouseDev.device.GetAddressOf(),
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("マウスデバイスの作成に失敗しました。製品名: {} 種別: {:X}",
				mouseDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(mouseDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// 機能情報を取得
		mouseDev.capability.dwSize = static_cast<DWORD>(sizeof(mouseDev.capability));
		hr = mouseDev.device->GetCapabilities(&mouseDev.capability);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("マウスデバイスの機能情報取得に失敗しました。製品名: {} 種別: {:X}",
				mouseDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(mouseDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// データフォーマット設定
		hr = mouseDev.device->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("マウスデバイスのフォーマットの設定に失敗しました。製品名: {} 種別: {:X}",
				mouseDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(mouseDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// 協調モード設定
		hr = mouseDev.device->SetCooperativeLevel(mouse->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("マウスデバイスの協調モード設定に失敗しました。製品名: {} 種別: {:X}",
				mouseDev.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(mouseDev.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// 相対値モードに設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = static_cast<DWORD>(sizeof(diprop));
		diprop.diph.dwHeaderSize = static_cast<DWORD>(sizeof(diprop.diph));
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;
		hr = mouseDev.device->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("マウスデバイスの軸モード設定に失敗しました。");
			exit(EXIT_FAILURE);
		}

		mouse->mouses_.push_back(std::move(mouseDev));

		return DIENUM_CONTINUE;
	}
}