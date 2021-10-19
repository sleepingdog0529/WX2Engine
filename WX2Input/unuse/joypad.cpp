#include "joypad.h"
#include <ranges>

namespace wx2
{
	Joypad::Joypad() :
		directInput_(),
		hwnd_()
	{
	}

	Joypad::~Joypad()
	{
		for (auto& jp : joypads_)
		{
			if (jp.device)
			{
				jp.device->Unacquire();
			}
		}
	}

	void Joypad::Initialize(DInputPtr directInput, HWND hwnd)
	{
		WX2_ASSERT_MSG(directInput, "IDirectInputDevice8がnullptrです。");
		WX2_ASSERT_MSG(IsWindowEnabled(hwnd) != 0, "ウィンドウハンドルが無効です。");
		directInput_ = directInput;
		hwnd_ = hwnd;
	}

	void Joypad::Regist()
	{
		HRESULT hr;

		hr = directInput_->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			SetupJoypadCallback,
			this,
			DIEDFL_ATTACHEDONLY);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイスティックデバイスの作成に失敗しました。");
			exit(EXIT_FAILURE);
		}
	}

	void Joypad::Update()
	{
		HRESULT hr;

		for (auto& jd : joypads_)
		{
			hr = jd.device->Poll();
			if (FAILED(hr))
			{
				// 切断されていた場合再登録を試みる
				hr = jd.device->Acquire();
				while (hr == DIERR_INPUTLOST)
				{
					hr = jd.device->Acquire();
				}

				std::memset(&jd.state, 0, sizeof(jd.state));
				continue;
			}

			hr = jd.device->GetDeviceState(sizeof(jd.state), &jd.state);
			if (FAILED(hr))
			{
				WX2_LOG_WARN("ジョイスティック状態取得に失敗しました。");
				continue;
			}
		}
	}

	BOOL Joypad::SetupJoypadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
	{
		WX2_ASSERT_MSG(
			GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_GAMEPAD ||
			GET_DIDEVICE_TYPE(lpddi->dwDevType) == DI8DEVTYPE_JOYSTICK,
			"ジョイパッドと異なるタイプのデバイスは列挙できません");

		auto* joypad = reinterpret_cast<Joypad*>(pvRef);
		WX2_ASSERT_MSG(joypad, "Gamepadクラスがnullptrです。");

		// デバイス作成済みの場合スキップする
		if (std::ranges::any_of(joypad->joypads_,
			[&lpddi](const JoypadInfo& di) { return di.instance.guidInstance == lpddi->guidInstance; }))
		{
			return DIENUM_CONTINUE;
		}

		HRESULT hr;
		JoypadInfo joypadInfo = {};
		joypadInfo.instance = std::move(static_cast<DIDEVICEINSTANCE>(*lpddi));

		// デバイス作成
		hr = joypad->directInput_->CreateDevice(
			joypadInfo.instance.guidInstance,
			joypadInfo.device.GetAddressOf(),
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスの作成に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		joypadInfo.capability.dwSize = sizeof(joypadInfo.capability);
		hr = joypadInfo.device->GetCapabilities(&joypadInfo.capability);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスの機能情報取得に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイスティックデバイスのフォーマットの設定に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->SetCooperativeLevel(joypad->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスの協調モード設定に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		// 絶対軸に設定
		DIPROPDWORD diprop = {};
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = 1;
		hr = joypadInfo.device->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスのバッファサイズ設定に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->EnumObjects(SetupAxisCallback, joypadInfo.device.Get(), DIDFT_AXIS);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスの軸設定に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->Acquire();
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスの制御開始に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		hr = joypadInfo.device->Poll();
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスのポーリング開始に失敗しました。製品名\"{}\" 種別\"{:X}\"",
				joypadInfo.instance.tszProductName,
				GET_DIDEVICE_SUBTYPE(joypadInfo.instance.dwDevType));
			return DIENUM_CONTINUE;
		}

		joypad->joypads_.push_back(std::move(joypadInfo));

		// ジョイパッドの最大数に到達したら列挙を終了する
		if (joypad->joypads_.size() >= MAX_JOYPAD_)
		{
			return DIENUM_STOP;
		}

		return DIENUM_CONTINUE;
	}

	BOOL CALLBACK Joypad::SetupAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
	{
		auto* device = reinterpret_cast<IDirectInputDevice8*>(pvRef);
		WX2_ASSERT_MSG(device, "ジョイパッドデバイスがnullptrです。");

		HRESULT hr;

		// 軸範囲を設定
		DIPROPRANGE diprg = {};
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = lpddoi->dwType;
		diprg.diph.dwHow = DIPH_BYID;
		diprg.lMin = -STICK_MAX_MAGNITUDE_;
		diprg.lMax = STICK_MAX_MAGNITUDE_;
		hr = device->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスの軸の範囲設定に失敗しました。");
			return DIENUM_CONTINUE;
		}

		// 絶対軸に設定
		DIPROPDWORD dropdw = {};
		dropdw.diph.dwSize = sizeof(dropdw);
		dropdw.diph.dwHeaderSize = sizeof(dropdw.diph);
		dropdw.diph.dwHow = DIPH_DEVICE;
		dropdw.dwData = DIPROPAXISMODE_ABS;
		hr = device->SetProperty(DIPROP_AXISMODE, &dropdw.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスの軸のモード設定に失敗しました。");
			return DIENUM_CONTINUE;
		}

		// デッドゾーンを設定
		dropdw.dwData = STICK_DEADZONE_;
		hr = device->SetProperty(DIPROP_DEADZONE, &dropdw.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスの軸のデッドゾーン設定に失敗しました。");
			return DIENUM_CONTINUE;
		}

		// 飽和ゾーンを設定
		dropdw.dwData = STICK_SATURATION_;
		hr = device->SetProperty(DIPROP_SATURATION, &dropdw.diph);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("ジョイパッドデバイスの軸の飽和ゾーン設定に失敗しました。");
			return DIENUM_CONTINUE;
		}

		return DIENUM_CONTINUE;
	}
}