#include "mouse.h"
#include "input.h"

namespace wx2
{
	void Mouse::Initialize(HWND hwnd)
	{
		auto* direct_input = Input::GetDirectInput();
		WX2_ASSERT_MSG(direct_input, "inputクラスが初期化されていません。");

		HRESULT hr;

		hr = direct_input->CreateDevice(
			GUID_SysMouse, 
			&device_, 
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("マウスデバイスの作成に失敗しました。");
			exit(EXIT_FAILURE);
		}

		hr = device_->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("マウスデバイスのフォーマットの設定に失敗しました。");
			exit(EXIT_FAILURE);
		}

		hr = device_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("マウスデバイスの協調モードの設定に失敗しました。");
			exit(EXIT_FAILURE);
		}

		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;
		device_->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		hr = device_->Acquire();
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("マウスデバイスの制御の開始に失敗しました。");
			exit(EXIT_FAILURE);
		}

		StateReset();
	}

	void Mouse::Finalize()
	{
		device_->Unacquire();
	}

	void Mouse::Update()
	{
		if (FAILED(device_->GetDeviceState(sizeof(DIMOUSESTATE2), &state_)))
		{
			if (FAILED(device_->Acquire()) ||
				FAILED(device_->GetDeviceState(sizeof(DIMOUSESTATE2), &state_)))
			{
				StateReset();
				return;
			}
		}

		buttons_[1] = std::move(buttons_[0]);

		for (int i = 0; i < NUM_BUTTONS; ++i)
		{
			buttons_[0][i] = state_.rgbButtons[i] & 0x80;
		}

		cursor_.x() = static_cast<float>(state_.lX) * CURSOR_SENSITIVITY_;
		cursor_.y() = static_cast<float>(state_.lY) * -CURSOR_SENSITIVITY_;

		scroll_ = static_cast<float>(Sign(state_.lZ)) * SCOLL_SENSITIVITY_;
	}

	void Mouse::StateReset()
	{
		std::memset(&buttons_, 0, sizeof(buttons_));
	}
}