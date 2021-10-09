#include "window.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>

namespace wx2
{
	LRESULT CALLBACK handle_msg_redirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT CALLBACK handle_msg_setup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	window::window(window_container* container, const window_property& window_prop) :
		hinst_(GetModuleHandle(nullptr)),
		window_prop_(window_prop)
	{
		// ランダムにユニークIDを生成
		const boost::uuids::uuid id = boost::uuids::random_generator()();
		const std::string uuid = boost::lexical_cast<std::string>(id);
		class_name_ = std::format("wx2eg-{}", uuid);

		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.lpfnWndProc = handle_msg_setup;
		wcex.hInstance = hinst_;
		wcex.lpszClassName = class_name_.c_str();
		if (!RegisterClassEx(&wcex))
		{
			WX2_LOG_ERROR("ウィンドウクラスの登録に失敗しました。code\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		hwnd_ = CreateWindow(
			class_name_.c_str(),
			window_prop_.title.c_str(),
			WS_OVERLAPPED | WS_CAPTION,
			window_prop_.x,
			window_prop_.y,
			window_prop_.width,
			window_prop_.height,
			nullptr,
			nullptr,
			hinst_,
			container
		);
		if (!hwnd_)
		{
			WX2_LOG_ERROR("ウィンドウの作成に失敗しました。code\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		ShowWindow(hwnd_, SW_SHOW);
		UpdateWindow(hwnd_);
	}

	window::~window()
	{
		if (hwnd_)
		{
			UnregisterClass(class_name_.c_str(), hinst_);
			DestroyWindow(hwnd_);
		}
	}

	LRESULT CALLBACK handle_msg_redirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		default:
		{
			window_container* const wnd_container = 
				reinterpret_cast<window_container*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			return wnd_container->wnd_proc(hwnd, msg, wp, lp);
		}
		}
	}

	LRESULT CALLBACK handle_msg_setup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_NCCREATE:
		{
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lp);
			window_container* wnd_container = 
				reinterpret_cast<window_container*>(create->lpCreateParams);

			if (!wnd_container)
			{
				WX2_LOG_CRITICAL("windows_containerはnullptrです。");
				exit(EXIT_FAILURE);
			}

			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd_container));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(handle_msg_redirect));

			return wnd_container->wnd_proc(hwnd, msg, wp, lp);
		}
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
		}
	}
}