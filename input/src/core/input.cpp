#include "input.h"

namespace wx2
{
	bool Input::Initialize(const HWND hwnd) noexcept
	{
		try
		{
			// DirectInput8デバイスを作成
			const HRESULT hr = DirectInput8Create(
				GetModuleHandle(nullptr),
				DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				&directInput_,
				nullptr);
			WX2_COM_ERROR_IF_FAILED(hr, "DirectInput8の作成に失敗しました。");

			// 各入力クラスの初期化
			keyboard_.Initialize(directInput_, hwnd);
			mouse_.Initialize(directInput_, hwnd);

			// 各入力クラスで接続中のデバイスを登録
			keyboard_.Regist();
			mouse_.Regist();

			return true;
		}
		catch(const COMException& exception)
		{
			WX2_LOG_ERROR(exception.what());
			return false;
		}
	}

	void Input::Update() noexcept
	{
		// 各入力状態の更新
		keyboard_.Update();
		mouse_.Update();
		gamepad_.Update();
	}
}