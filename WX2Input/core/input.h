/*********************************************************************
 * @file	input.h
 * @author	Tomomi Murakami
 * @date	2021/10/11 23:47
 * @brief	�f�o�C�X���͋��ʃN���X
*********************************************************************/
#pragma once
#pragma warning(push, 0)
#include "gamepad.h"
#include <dinput.h>
#pragma warning(pop)

namespace wx2
{
	/// <summary>
	/// �f�o�C�X���͂̋��ʂ̏������s���N���X
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

	private:
		DInputPtr directInput_;
		Gamepad gamepad_;
	};
}