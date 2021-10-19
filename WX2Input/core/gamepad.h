/*********************************************************************
 * @file	gamepad.h
 * @author	Tomomi Murakami
 * @date	2021/10/17 23:43
 * @brief	XInput対応コントローラー入力
*********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <Xinput.h>
#include <wrl/client.h>
#include <bitset>
#include <array>
#pragma warning(pop)

namespace wx2
{
	class Gamepad
	{
	public:
		enum Buttons
		{
			DPadUp = 0,
			DPadDown,
			DPadLeft,
			DPadRight,
			Start,
			Back,
			LThumb,
			RThumb,
			LShoulder,
			RShoulder,
			A,
			B,
			X,
			Y,
		};

		enum Axises
		{
			LTrigger = 0,
			RTrigger,
			LThumbX,
			LThumbY,
			RThumbX,
			RThumbY,
		};

	private:
		// 左スティックのデッドゾーン(0 ~ 1)
		static constexpr float LEFT_THUMB_DEADZONE_ = 
			XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)SHRT_MAX; 

		// 左スティックのデッドゾーン(0 ~ 1)
		static constexpr float RIGHT_THUMB_DEADZONE_ =
			XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / (float)SHRT_MAX; 

		// 両トリガーのデッドゾーン(0 ~ 1)
		static constexpr float TRIGGER_DEADZONE_ =
			XINPUT_GAMEPAD_TRIGGER_THRESHOLD / (float)UCHAR_MAX;

		static constexpr size_t NUM_BUTTONS_ = 14;
		static constexpr size_t NUM_AXISES_ = 6;

		struct GamepadState
		{
			struct
			{
				std::bitset<NUM_BUTTONS_> buttons;
				std::array<float, NUM_AXISES_> axises;
			} current, previous;
			bool isConnected;
		};

	public:
		Gamepad();
		virtual ~Gamepad() = default;

		WX2_DISALLOW_COPY_AND_MOVE(Gamepad);

		void Update();

		bool IsDown(Buttons button, size_t user = 0) const;
		bool IsPressed(Buttons button, size_t user = 0) const;
		bool IsReleased(Buttons button, size_t user = 0) const;
		float GetAxisValue(Axises axises, size_t user = 0) const;
		float GetAxisVelocity(Axises axises, size_t user = 0) const;

	private:
		std::array<GamepadState, XUSER_MAX_COUNT> states_;
		XINPUT_STATE buffer_;
	};
}