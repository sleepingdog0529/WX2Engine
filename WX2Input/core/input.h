/*********************************************************************
 * @file	input.h
 * @author	Tomomi Murakami
 * @date	2021/10/11 23:47
 * @brief	�f�o�C�X���͋��ʃN���X
*********************************************************************/
#pragma once
#pragma warning(push, 0) 
#include <dinput.h>
#include <wrl/client.h>
#pragma warning(pop)

namespace wx2
{
	/// <summary>
	/// �f�o�C�X���͂̋��ʂ̏������s���N���X
	/// </summary>
	class Input
	{
	public:
		/// <summary>
		/// ���̓f�o�C�X�̏�����
		/// </summary>
		static void Initialize();

		static IDirectInput8* GetDirectInput() { return directInput_.Get(); }

	private:
		static inline Microsoft::WRL::ComPtr<IDirectInput8> directInput_;
	};
}