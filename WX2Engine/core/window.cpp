#include "window.h"

namespace wx2
{
	Window::Window(WindowContainer* container, WindowPropertyPtr windowProp) :
		container_(container),
		windowProp_(windowProp)
	{
		WX2_ASSERT(windowProp_->width > 0);
		WX2_ASSERT(windowProp_->height > 0);

		// ランダムにユニークIDを生成
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::stringstream uuid_ss;
		uuid_ss << uuid;
		className_ = std::format("wx2eg-{}", uuid_ss.str());

		// インスタンスハンドル取得
		const HINSTANCE hinst = GetModuleHandle(nullptr);

		// ウィンドウクラスを登録
		WNDCLASSEX wcex = {};
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.cbSize = sizeof(wcex);
		wcex.lpfnWndProc = HandleMessageSetup;
		wcex.hInstance = hinst;
		wcex.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wcex.lpszClassName = className_.c_str();
		if (!RegisterClassEx(&wcex))
		{
			WX2_LOG_ERROR("ウィンドウクラスの登録に失敗しました。エラーコード:\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// ウィンドウを作成
		hwnd_ = CreateWindow(
			className_.c_str(),
			windowProp_->title.c_str(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			windowProp_->x,
			windowProp_->y,
			windowProp_->width,
			windowProp_->height,
			nullptr,
			nullptr,
			hinst,
			this
		);
		if (!hwnd_)
		{
			WX2_LOG_ERROR("ウィンドウの作成に失敗しました。エラーコード:\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// ウィンドウを可視化、更新
		UpdateWindow(hwnd_);
	}

	Window::~Window()
	{
		// ウィンドウを登録解除し、破棄する
		if (hwnd_)
		{
			UnregisterClass(className_.c_str(), GetModuleHandle(nullptr));
			DestroyWindow(hwnd_);
		}
	}

	LRESULT CALLBACK Window::HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		// パラメータからウィンドウコンテナにキャストする
		Window* const window =
			reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		switch (msg)
		{
			case WM_MOVE:
			{
				window->windowProp_->x = static_cast<int>(LOWORD(lp));
				window->windowProp_->y = static_cast<int>(HIWORD(lp));
				return 0;
			}
			case WM_SIZE:
			{
				window->windowProp_->width = static_cast<int>(LOWORD(lp));
				window->windowProp_->height = static_cast<int>(HIWORD(lp));

				if (wp == SIZE_MAXIMIZED)
				{
					window->windowProp_->fullscreen = true;
				}
				else if (wp == SIZE_RESTORED)
				{
					window->windowProp_->fullscreen = false;
				}

				return 0;
			}
			case WM_CLOSE:
			{
				DestroyWindow(hwnd);
				return 0;
			}
		}

		// 動的なウィンドウプロシージャに処理を委譲する
		return window->container_->WindowProcedure(hwnd, msg, wp, lp);
	}

	LRESULT CALLBACK Window::HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		// パラメータからウィンドウコンテナにキャストする
		const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lp);
		Window* window = reinterpret_cast<Window*>(create->lpCreateParams);

		if(msg == WM_NCCREATE)
		{
			if (!window)
			{
				WX2_LOG_CRITICAL("windows_containerはnullptrです。");
				exit(EXIT_FAILURE);
			}

			// ウィンドウプロシージャを差し替え
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));

			// 動的なウィンドウプロシージャに処理を委譲する
			return window->container_->WindowProcedure(hwnd, msg, wp, lp);
		}

		return DefWindowProc(hwnd, msg, wp, lp);
	}
}