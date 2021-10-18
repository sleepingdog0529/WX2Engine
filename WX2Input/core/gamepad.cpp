#include "gamepad.h"
#include <algorithm>
#include <iostream>

namespace wx2
{
	Gamepad::Gamepad()
	{
		std::memset(&states_, 0, sizeof(states_));
	}

	void Gamepad::Update()
	{
		for (auto& [current, prev, connection] : states_)
		{
			prev = std::move(current);
			memset(&current, 0, sizeof(current));

			connection = XInputGetState(0, &buffer_) == ERROR_SUCCESS;
			if (connection)
			{
				current.buttons[DPadUp]	   = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
				current.buttons[DPadDown]  = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
				current.buttons[DPadLeft]  = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
				current.buttons[DPadRight] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
				current.buttons[Start]     = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_START;
				current.buttons[Back]      = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
				current.buttons[LThumb]    = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
				current.buttons[RThumb]    = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
				current.buttons[LShoulder] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
				current.buttons[RShoulder] = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
				current.buttons[A]         = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_A;
				current.buttons[B]         = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_B;
				current.buttons[X]         = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_X;
				current.buttons[Y]         = buffer_.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

				const float lt = buffer_.Gamepad.bLeftTrigger  / (float)UCHAR_MAX;
				const float rt = buffer_.Gamepad.bRightTrigger / (float)UCHAR_MAX;
				const float lx = buffer_.Gamepad.sThumbLX / (float)SHRT_MAX;
				const float ly = buffer_.Gamepad.sThumbLY / (float)SHRT_MAX;
				const float rx = buffer_.Gamepad.sThumbRX / (float)SHRT_MAX;
				const float ry = buffer_.Gamepad.sThumbRY / (float)SHRT_MAX;

				current.axises[LTrigger] = std::clamp(Remap(lt, TRIGGER_DEADZONE_, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f);
				current.axises[RTrigger] = std::clamp(Remap(rt, TRIGGER_DEADZONE_, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f);
				current.axises[LThumbX]  = std::clamp(Remap(std::fabsf(lx), LEFT_THUMB_DEADZONE_, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(lx);
				current.axises[LThumbY]  = std::clamp(Remap(std::fabsf(ly), LEFT_THUMB_DEADZONE_, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(ly);
				current.axises[RThumbX]  = std::clamp(Remap(std::fabsf(rx), RIGHT_THUMB_DEADZONE_, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(rx);
				current.axises[RThumbY]  = std::clamp(Remap(std::fabsf(ry), RIGHT_THUMB_DEADZONE_, 1.0f, 0.0f, 1.0f), 0.0f, 1.0f) * Sign(ry);
			}
		}
	}

	bool Gamepad::IsDown(Buttons button, size_t user) const
	{
		return states_[user].current.buttons[button];
	}

	bool Gamepad::IsPressed(Buttons button, size_t user) const
	{
		return states_[user].current.buttons[button] & !states_[user].prev.buttons[button];
	}

	bool Gamepad::IsReleased(Buttons button, size_t user) const
	{
		return !states_[user].current.buttons[button] & states_[user].prev.buttons[button];
	}

	float Gamepad::GetAxisValue(Axises axises, size_t user) const
	{
		return states_[user].current.axises[axises];
	}

	float Gamepad::GetAxisAcceleration(Axises axises, size_t user) const
	{
		return states_[user].current.axises[axises] - states_[user].prev.axises[axises];
	}
}