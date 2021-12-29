#include "input.h"

namespace wx2
{
	bool Input::Initialize(const HWND hwnd) noexcept
	{
		try
		{
			// DirectInput8�f�o�C�X���쐬
			const HRESULT hr = DirectInput8Create(
				GetModuleHandle(nullptr),
				DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				&directInput_,
				nullptr);
			WX2_COM_ERROR_IF_FAILED(hr, "DirectInput8�̍쐬�Ɏ��s���܂����B");

			// �e���̓N���X�̏�����
			keyboard_.Initialize(directInput_, hwnd);
			mouse_.Initialize(directInput_, hwnd);

			// �e���̓N���X�Őڑ����̃f�o�C�X��o�^
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
		// �e���͏�Ԃ̍X�V
		keyboard_.Update();
		mouse_.Update();
		gamepad_.Update();
	}
}