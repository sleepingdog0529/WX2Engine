/*********************************************************************
 * @file	input.h
 * @author	Tomomi Murakami
 * @date	2021/10/11 23:47
 * @brief	デバイス入力共通クラス
*********************************************************************/
#pragma once
#pragma warning(push, 0) 
#include <dinput.h>
#include <wrl/client.h>
#pragma warning(pop)

namespace wx2
{
	/// <summary>
	/// デバイス入力の共通の処理を行うクラス
	/// </summary>
	class Input
	{
	public:
		/// <summary>
		/// 入力デバイスの初期化
		/// </summary>
		static void Initialize();

		static IDirectInput8* GetDirectInput() { return directInput_.Get(); }

	private:
		static inline Microsoft::WRL::ComPtr<IDirectInput8> directInput_;
	};
}