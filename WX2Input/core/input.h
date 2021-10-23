/*********************************************************************
 * @file	input.h
 * @author	Tomomi Murakami
 * @date	2021/10/11 23:47
 * @brief	デバイス入力共通クラス
*********************************************************************/
#pragma once
#include "keyboard.h"
#include "mouse.h"
#include "gamepad.h"
#pragma warning(push, 0)
#include <dinput.h>
#pragma warning(pop)

namespace wx2
{
	/// <summary>
	/// デバイス入力の共通の処理を行うクラス
	/// </summary>
	class Input
	{
	private:
		using DInputPtr = Microsoft::WRL::ComPtr<IDirectInput8>;

	public:
		Input();
		virtual ~Input() = default;

		void Initialize(HWND hwnd);
		void Update();

		bool IsDown(Keyboard::Keys key) const { return keyboard_.IsDown(key); }
		bool IsPressed(Keyboard::Keys key) const { return keyboard_.IsPressed(key); }
		bool IsReleased(Keyboard::Keys key) const { return keyboard_.IsReleased(key); }

		bool IsDown(Mouse::Buttons button) const { return mouse_.IsDown(button); }
		bool IsPressed(Mouse::Buttons button) const { return mouse_.IsPressed(button); }
		bool IsReleased(Mouse::Buttons button) const { return mouse_.IsReleased(button); }
		float GetAxisVelocity(Mouse::Axises axises) const { return mouse_.GetAxisVelocity(axises); }

		bool IsDown(Gamepad::Buttons button) const { return gamepad_.IsDown(button); }
		bool IsPressed(Gamepad::Buttons button) const { return gamepad_.IsPressed(button); }
		bool IsReleased(Gamepad::Buttons button) const { return gamepad_.IsReleased(button); }
		float GetAxisValue(Gamepad::Axises axises) const { return gamepad_.GetAxisVelocity(axises); }
		float GetAxisVelocity(Gamepad::Axises axises) const { return gamepad_.GetAxisVelocity(axises); }

	private:
		DInputPtr directInput_;

		Keyboard keyboard_;
		Mouse mouse_;
		Gamepad gamepad_;
	};
}