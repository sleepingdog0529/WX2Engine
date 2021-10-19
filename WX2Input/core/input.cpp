#include "input.h"
#include <algorithm>
#include <iostream>
#include <ranges>

namespace wx2
{
	Input::Input() :
		directInput_()
	{
	}

	void Input::Initialize(HWND hwnd)
	{
		HRESULT hr;
		hr = DirectInput8Create(
			GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			&directInput_,
			nullptr);
		if (FAILED(hr))
		{
			WX2_LOG_ERROR("DirectInput8ÇÃçÏê¨Ç…é∏îsÇµÇ‹ÇµÇΩÅB");
			exit(EXIT_FAILURE);
		}

		mouse_.Initialize(directInput_, hwnd);
		mouse_.Regist();
	}

	void Input::Update()
	{
		gamepad_.Update();
		mouse_.Update();
	}
}