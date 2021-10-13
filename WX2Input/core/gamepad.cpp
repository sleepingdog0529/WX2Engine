//#include "gamepad.h"
//#include "input.h"
//
//namespace wx2
//{
//	void Gamepad::Initialize(HWND hwnd)
//	{
//		auto* direct_input = Input::GetDirectInput();
//		WX2_ASSERT_MSG(direct_input, "Inputクラスが初期化されていません。");
//
//		HRESULT hr;
//
//		hr = direct_input->EnumDevices(
//			DI8DEVTYPE_JOYSTICK,
//			EnumJoyStickCallBack, 
//			hwnd, 
//			DIEDFL_ATTACHEDONLY);
//		if (FAILED(hr))
//		{
//			WX2_LOG_ERROR("ジョイスティックデバイスの作成に失敗しました。");
//			exit(EXIT_FAILURE);
//		}
//
//		hr = device_->SetDataFormat(&c_dfDIJoystick2);
//		if (FAILED(hr))
//		{
//			WX2_LOG_ERROR("ジョイスティックデバイスのフォーマットの設定に失敗しました。");
//			exit(EXIT_FAILURE);
//		}
//
//		hr = device_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//		if (FAILED(hr))
//		{
//			WX2_LOG_ERROR("ジョイスティックデバイスの協調モード設定に失敗しました。");
//			exit(EXIT_FAILURE);
//		}
//
//		hr = device_->EnumObjects(EnumAxesCallback, nullptr, DIDFT_AXIS);
//		if (FAILED(hr))
//		{
//			WX2_LOG_ERROR("ジョイスティックデバイスの軸範囲設定に失敗しました。");
//			exit(EXIT_FAILURE);
//		}
//
//		DIPROPDWORD diprop;
//		diprop.diph.dwSize = sizeof(diprop);
//		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
//		diprop.diph.dwObj = 0;
//		diprop.diph.dwHow = DIPH_DEVICE;
//		diprop.dwData = DIPROPAXISMODE_ABS;
//		hr = device_->SetProperty(DIPROP_AXISMODE, &diprop.diph);
//		if (FAILED(hr))
//		{
//			WX2_LOG_ERROR("ジョイスティックデバイスの軸モード設定に失敗しました。");
//			exit(EXIT_FAILURE);
//		}
//
//		diprop.dwData = 100;
//		hr = device_->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
//		if (FAILED(hr))
//		{
//			WX2_LOG_ERROR("ジョイスティックデバイスのバッファサイズ設定に失敗しました。");
//			exit(EXIT_FAILURE);
//		}
//
//		hr = device_->Acquire();
//		if (FAILED(hr))
//		{
//			WX2_LOG_ERROR("ジョイスティックデバイスの制御開始に失敗しました。");
//			exit(EXIT_FAILURE);
//		}
//
//		StateReset();
//	}
//
//	void Gamepad::Finalize()
//	{
//		device_->Unacquire();
//	}
//
//	void Gamepad::Update()
//	{
//		buttons_[1] = std::move(buttons_[0]);
//
//		if (FAILED(device_->GetDeviceState(sizeof(state_), &state_)))
//		{
//			// 切断されていた場合再登録を試みる
//			if (FAILED(device_->Acquire()) ||
//				FAILED(device_->GetDeviceState(sizeof(state_), &state_)))
//			{
//				StateReset();
//				return;
//			}
//		}
//	}
//
//	void Gamepad::StateReset()
//	{
//	}
//
//	BOOL Gamepad::EnumDeviceObjCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
//	{
//		auto direct_input = Input::GetDirectInput();
//		WX2_ASSERT_MSG(direct_input, "Inputクラスが初期化されていません。");
//
//		auto& device = devices_.emplace_back();
//		std::string productName(ipddi->tszProductName);
//
//		HRESULT hr;
//
//		hr = direct_input->CreateDevice(
//			ipddi->guidInstance, 
//			device.GetAddressOf(),
//			nullptr);
//		if (FAILED(hr))
//		{
//			WX2_LOG_TRACE("ジョイスティックデバイスの作成に失敗しました。");
//		}
//
//		hr = device->SetDataFormat(&c_dfDIJoystick2);
//		if (FAILED(hr))
//		{
//			WX2_LOG_TRACE("ジョイスティックデバイスのフォーマットの設定に失敗しました。");
//		}
//
//		HWND hwnd = static_cast<HWND>(pvRef);
//
//		hr = device->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//		if (FAILED(hr))
//		{
//			WX2_LOG_TRACE("ジョイスティックデバイスの協調モード設定に失敗しました。");
//		}
//
//		return DIENUM_CONTINUE;
//	}
//
//	BOOL CALLBACK Gamepad::EnumJoyStickCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
//	{
//		auto direct_input = Input::GetDirectInput();
//		WX2_ASSERT_MSG(direct_input, "Inputクラスが初期化されていません。");
//
//		HRESULT hr;
//
//		hr = direct_input->CreateDevice(ipddi->guidInstance, device_.GetAddressOf(), nullptr);
//		if (FAILED(hr))
//		{
//			WX2_LOG_TRACE("ジョイスティックデバイスの作成に失敗しました。");
//			return DIENUM_CONTINUE;
//		}
//
//		DIDEVCAPS devcaps = {};
//		devcaps.dwSize = sizeof(devcaps);
//		hr = device_->GetCapabilities(&devcaps);
//		if (FAILED(hr))
//		{
//			WX2_LOG_TRACE("ジョイスティックデバイスの能力取得に失敗しました。");
//			return DIENUM_CONTINUE;
//		}
//
//		return DIENUM_STOP;
//	}
//
//	BOOL CALLBACK Gamepad::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE ipddoi, LPVOID pvRef)
//	{
//		// 軸の値の範囲を設定
//		DIPROPRANGE diprg = {};
//		diprg.diph.dwSize = sizeof(diprg);
//		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
//		diprg.diph.dwObj = ipddoi->dwType;
//		diprg.diph.dwHow = DIPH_BYID;
//		diprg.lMin = -MAX_STICK_MAGNITUDE_;
//		diprg.lMax = MAX_STICK_MAGNITUDE_;
//
//		if (FAILED(device_->SetProperty(DIPROP_RANGE, &diprg.diph)))
//		{
//			WX2_LOG_TRACE("ジョイスティック軸デバイスの軸範囲設定に失敗しました。");
//			return DIENUM_STOP;
//		}
//
//		return DIENUM_CONTINUE;
//	}
//}