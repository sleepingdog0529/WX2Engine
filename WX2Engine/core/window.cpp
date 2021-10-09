#include "window.h"

namespace wx2
{
	LRESULT CALLBACK handle_msg_redirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT CALLBACK handle_msg_setup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	window::window(window_container* container, const window_property& window_prop) :
		container_(container),
		window_prop_(window_prop)
	{
		// ランダムにユニークIDを生成
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::stringstream uuid_ss;
		uuid_ss << uuid;
		class_name_ = std::format("wx2eg-{}", uuid_ss.str());

		// ウィンドウクラスを登録
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.lpfnWndProc = handle_msg_setup;
		wcex.hInstance = container_->get_instance_handle();
		wcex.lpszClassName = class_name_.c_str();
		if (!RegisterClassEx(&wcex))
		{
			WX2_LOG_ERROR("ウィンドウクラスの登録に失敗しました。code\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// ウィンドウを作成
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
			container_->get_instance_handle(),
			container_
		);
		if (!hwnd_)
		{
			WX2_LOG_ERROR("ウィンドウの作成に失敗しました。code\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// ウィンドウを可視化、更新
		ShowWindow(hwnd_, SW_SHOW);
		UpdateWindow(hwnd_);
	}

	window::~window()
	{
		// ウィンドウを登録解除し、破棄する
		if (hwnd_)
		{
			UnregisterClass(class_name_.c_str(), container_->get_instance_handle());
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
			// パラメータからウィンドウコンテナにキャストする
			window_container* const wnd_container = 
				reinterpret_cast<window_container*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			// 動的なウィンドウプロシージャに処理を委譲する
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
			// パラメータからウィンドウコンテナにキャストする
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lp);
			window_container* wnd_container = 
				reinterpret_cast<window_container*>(create->lpCreateParams);

			if (!wnd_container)
			{
				WX2_LOG_CRITICAL("windows_containerはnullptrです。");
				exit(EXIT_FAILURE);
			}

			// ウィンドウプロシージャを差し替え
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd_container));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(handle_msg_redirect));

			// 動的なウィンドウプロシージャに処理を委譲する
			return wnd_container->wnd_proc(hwnd, msg, wp, lp);
		}
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
		}
	}
}