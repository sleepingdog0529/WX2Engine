#include "window_container.h"

namespace wx2 
{
	window_container::window_container() :
		hinst_(GetModuleHandle(nullptr))
	{
	}

	std::shared_ptr<window> window_container::create(const std::string& name, const window_property& window_prop)
	{
		auto [itr, success] = render_windows_.emplace(name, std::make_shared<window>(this, window_prop));

		if (!success)
		{
			WX2_LOG_CRITICAL("ウィンドウ名が重複しています。ウィンドウ名\"{}\"", name);
			exit(EXIT_FAILURE);
		}

		return itr->second;
	}

	void window_container::process_msgs(std::function<bool()> process)
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

	LRESULT window_container::wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}