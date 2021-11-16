/*********************************************************************
 * @file	input.h
 * @author	Tomomi Murakami
 * @date	2021/10/11 23:47
 * @brief	デバイス入力共通クラス
*********************************************************************/
#pragma once
#include "WX2Common.h"
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
	class Input final
	{
	private:
		using DInputPtr = Microsoft::WRL::ComPtr<IDirectInput8>;

	public:
		Input() = default;
		~Input() = default;

		WX2_DISALLOW_COPY_AND_MOVE(Input);

		void Initialize(const HWND hwnd);
		void Update();

		[[nodiscard]] bool IsDown(const Keyboard::Keys key) const { return keyboard_.IsDown(key); }
		[[nodiscard]] bool IsPressed(const Keyboard::Keys key) const { return keyboard_.IsPressed(key); }
		[[nodiscard]] bool IsReleased(const Keyboard::Keys key) const { return keyboard_.IsReleased(key); }

		[[nodiscard]] bool IsDown(const Mouse::Buttons button) const { return mouse_.IsDown(button); }
		[[nodiscard]] bool IsPressed(const Mouse::Buttons button) const { return mouse_.IsPressed(button); }
		[[nodiscard]] bool IsReleased(const Mouse::Buttons button) const { return mouse_.IsReleased(button); }
		[[nodiscard]] float GetAxisVelocity(const Mouse::Axises axises) const { return mouse_.GetAxisVelocity(axises); }

		[[nodiscard]] bool IsDown(const Gamepad::Buttons button) const { return gamepad_.IsDown(button); }
		[[nodiscard]] bool IsPressed(const Gamepad::Buttons button) const { return gamepad_.IsPressed(button); }
		[[nodiscard]] bool IsReleased(const Gamepad::Buttons button) const { return gamepad_.IsReleased(button); }
		[[nodiscard]] float GetAxisValue(const Gamepad::Axises axises) const { return gamepad_.GetAxisVelocity(axises); }
		[[nodiscard]] float GetAxisVelocity(const Gamepad::Axises axises) const { return gamepad_.GetAxisVelocity(axises); }

	private:
		DInputPtr directInput_;
		Keyboard keyboard_;
		Mouse mouse_;
		Gamepad gamepad_;
	};
}