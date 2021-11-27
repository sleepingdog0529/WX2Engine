#include "input.h"
#include <algorithm>
#include <iostream>
#include <ranges>

namespace wx2
{
	void Input::Initialize(const HWND hwnd) noexcept
	{
		if (const HRESULT hr = DirectInput8Create(
			GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			&directInput_,
			nullptr); FAILED(hr))
		{
			WX2_LOG_ERROR("DirectInput8ÇÃçÏê¨Ç…é∏îsÇµÇ‹ÇµÇΩÅB");
			exit(EXIT_FAILURE);
		}

		keyboard_.Initialize(directInput_, hwnd);
		mouse_.Initialize(directInput_, hwnd);

		keyboard_.Regist();
		mouse_.Regist();
	}

	void Input::Update() noexcept
	{
		keyboard_.Update();
		mouse_.Update();
		gamepad_.Update();
	}
}