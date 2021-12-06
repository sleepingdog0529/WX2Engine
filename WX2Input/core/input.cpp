#include "input.h"
#include <ranges>

namespace wx2
{
	bool Input::Initialize(const HWND hwnd) noexcept
	{
		if (FAILED(DirectInput8Create(
			GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			&directInput_,
			nullptr)))
		{
			WX2_LOG_ERROR("DirectInput8ÇÃçÏê¨Ç…é∏îsÇµÇ‹ÇµÇΩÅB");
			return false;
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