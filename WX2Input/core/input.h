/*********************************************************************
 * @file	input.h
 * @author	Tomomi Murakami
 * @date	2021/10/11 23:47
 * @brief	�f�o�C�X���͋��ʃN���X
*********************************************************************/
#pragma once
#include <WX2Common.h>
#include "keyboard.h"
#include "mouse.h"
#include "gamepad.h"
#pragma warning(push, 0)
#include <dinput.h>
#pragma warning(pop)

namespace wx2
{
	/**
	 * @brief �S�Ă̓��͊Ǘ�
	 */
	class Input final
	{
	private:
		using DInputPtr = Microsoft::WRL::ComPtr<IDirectInput8>;

	public:
		Input() = default;
		~Input() = default;

		WX2_DISALLOW_COPY_AND_MOVE(Input);

		/**
		 * @brief  �e���̓f�o�C�X�̏�����
		 * @param  hwnd ���͂��󂯕t����E�B���h�E�̃n���h��
		 * @return ����ɏ������ł�����
		 */
		bool Initialize(HWND hwnd) noexcept;


		void Update() noexcept;

		[[nodiscard]] const Keyboard& GetKeyboard() const noexcept { return keyboard_; }
		[[nodiscard]] const Mouse& GetMouse()       const noexcept { return mouse_; }
		[[nodiscard]] const Gamepad& GetGamepad()   const noexcept { return gamepad_; }

	private:
		//! DirectInput��
		DInputPtr directInput_;
		Keyboard keyboard_;
		Mouse mouse_;
		Gamepad gamepad_;
	};
}