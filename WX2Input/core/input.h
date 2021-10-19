/*********************************************************************
 * @file	input.h
 * @author	Tomomi Murakami
 * @date	2021/10/11 23:47
 * @brief	デバイス入力共通クラス
*********************************************************************/
#pragma once
#pragma warning(push, 0)
#include "mouse.h"
#include "gamepad.h"
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

		bool IsDown(Mouse::Buttons button) const { return mouse_.IsDown(button); }
		bool IsPressed(Mouse::Buttons button) const { return mouse_.IsReleased(button); }
		bool IsReleased(Mouse::Buttons button) const { return mouse_.IsReleased(button); }
		float GetAxisValue(Mouse::Axises axises) const { return mouse_.GetAxisVelocity(axises); }

	private:
		DInputPtr directInput_;
		Mouse mouse_;
		Gamepad gamepad_;
	};
}