#include "gamepad.h"
#include <algorithm>

namespace wx2
{
	Gamepad::Gamepad() noexcept
		: states_{}
		, buffer_()
	{
	}

	void Gamepad::Update() noexcept
	{
		// コントローラーの最大接続数分ループ
		for (auto& [curt, prev] : states_)
		{
			prev = curt;
			curt = {};

			// コントローラー状態取得
			if (SUCCEEDED(XInputGetState(0, &buffer_)))
			{
				// ボタン状態を格納
				curt.buttons[DPadUp]	= buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
				curt.buttons[DPadDown]  = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
				curt.buttons[DPadLeft]  = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
				curt.buttons[DPadRight] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
				curt.buttons[Start]     = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_START;
				curt.buttons[Back]      = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
				curt.buttons[LThumb]    = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
				curt.buttons[RThumb]    = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
				curt.buttons[LShoulder] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
				curt.buttons[RShoulder] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
				curt.buttons[A]         = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_A;
				curt.buttons[B]         = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_B;
				curt.buttons[X]         = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_X;
				curt.buttons[Y]         = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

				// 軸値をfloat型の0~1に変換
				const float lt = static_cast<float>(buffer_.Gamepad.bLeftTrigger)  / static_cast<float>(UCHAR_MAX);
				const float rt = static_cast<float>(buffer_.Gamepad.bRightTrigger) / static_cast<float>(UCHAR_MAX);
				const float lx = static_cast<float>(buffer_.Gamepad.sThumbLX) / static_cast<float>(SHRT_MAX);
				const float ly = static_cast<float>(buffer_.Gamepad.sThumbLY) / static_cast<float>(SHRT_MAX);
				const float rx = static_cast<float>(buffer_.Gamepad.sThumbRX) / static_cast<float>(SHRT_MAX);
				const float ry = static_cast<float>(buffer_.Gamepad.sThumbRY) / static_cast<float>(SHRT_MAX);

				// デッドゾーンを適応して軸値を格納
				curt.axises[LTrigger] = std::clamp(Remap(lt, TRIGGER_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f);
				curt.axises[RTrigger] = std::clamp(Remap(rt, TRIGGER_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f);
				curt.axises[LThumbX]  = std::clamp(Remap(std::fabsf(lx), LEFT_THUMB_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(lx);
				curt.axises[LThumbY]  = std::clamp(Remap(std::fabsf(ly), LEFT_THUMB_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(ly);
				curt.axises[RThumbX]  = std::clamp(Remap(std::fabsf(rx), RIGHT_THUMB_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(rx);
				curt.axises[RThumbY]  = std::clamp(Remap(std::fabsf(ry), RIGHT_THUMB_DEADZONE, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(ry);
			}
		}
	}
}