#include "window_container.h"

namespace wx2 
{
	std::shared_ptr<Window> WindowContainer::Create(const std::string& name, const WindowProperty& windowProp)
	{
		auto [itr, success] = windows_.emplace(name, std::make_shared<Window>(this, windowProp));

		if (!success)
		{
			WX2_LOG_CRITICAL("ウィンドウ名が重複しています。ウィンドウ名:\"{}\"", name);
			exit(EXIT_FAILURE);
		}

		return itr->second;
	}

	void WindowContainer::ProcessMessages(std::function<bool()> process)
	{
		MSG msg = {};

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if (!process()) break;
			}
		}
	}

	LRESULT WindowContainer::WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}