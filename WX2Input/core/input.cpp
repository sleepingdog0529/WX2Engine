#include "input.h"

namespace wx2
{
	void Input::Initialize()
	{
		HRESULT hr;

		hr = (DirectInput8Create(
			GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			static_cast<LPVOID*>(&directInput_),
			nullptr));
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("DirectInput8�̍쐬�Ɏ��s���܂����B");
			exit(EXIT_FAILURE);
		}
	}
}