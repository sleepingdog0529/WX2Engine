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
		explicit Input(HWND hwnd);
		void AcquireDevices();

		IDirectInput8* GetDirectInput() { return directInput_.Get(); }

	private:
		HWND hwnd_;
		Microsoft::WRL::ComPtr<IDirectInput8> directInput_;
		std::vector<DIDEVICEINSTANCE> devices_;
	};
}