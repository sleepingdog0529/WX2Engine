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

		bool Initialize(const HWND hwnd) noexcept;
		void Update() noexcept;

		[[nodiscard]] const Keyboard& GetKeyboard() const noexcept { return keyboard_; }
		[[nodiscard]] const Mouse& GetMouse()       const noexcept { return mouse_;    }
		[[nodiscard]] const Gamepad& GetGamepad()   const noexcept { return gamepad_;  }

	private:
		DInputPtr directInput_;
		Keyboard keyboard_;
		Mouse mouse_;
		Gamepad gamepad_;
	};
}