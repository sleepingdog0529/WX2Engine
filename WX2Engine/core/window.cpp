#include "window.h"

namespace wx2
{
	LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	Window::Window(WindowContainer* container, const WindowProperty& windowProp) :
		container_(container),
		windowProp_(windowProp)
	{
		WX2_ASSERT(windowProp_.width > 0);
		WX2_ASSERT(windowProp_.height > 0);

		// ランダムにユニークIDを生成
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::stringstream uuid_ss;
		uuid_ss << uuid;
		className_ = std::format("wx2eg-{}", uuid_ss.str());

		// インスタンスハンドル取得
		const HINSTANCE hinst = GetModuleHandle(nullptr);

		// ウィンドウクラスを登録
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.lpfnWndProc = HandleMessageSetup;
		wcex.hInstance = hinst;
		wcex.lpszClassName = className_.c_str();
		if (!RegisterClassEx(&wcex))
		{
			WX2_LOG_ERROR("ウィンドウクラスの登録に失敗しました。エラーコード:\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// ウィンドウを作成
		hwnd_ = CreateWindow(
			className_.c_str(),
			windowProp_.title.c_str(),
			WS_OVERLAPPED | WS_CAPTION,
			windowProp_.x,
			windowProp_.y,
			windowProp_.width,
			windowProp_.height,
			nullptr,
			nullptr,
			hinst,
			container_
		);
		if (!hwnd_)
		{
			WX2_LOG_ERROR("ウィンドウの作成に失敗しました。エラーコード:\"{}\"", GetLastError());
			exit(EXIT_FAILURE);
		}

		// ウィンドウを可視化、更新
		ShowWindow(hwnd_, SW_SHOW);
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

	LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		default:
		{
			// パラメータからウィンドウコンテナにキャストする
			WindowContainer* const wndContainer = 
				reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			// 動的なウィンドウプロシージャに処理を委譲する
			return wndContainer->WindowProcedure(hwnd, msg, wp, lp);
		}
		}
	}

	LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_NCCREATE:
		{
			// パラメータからウィンドウコンテナにキャストする
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lp);
			WindowContainer* wndContainer = 
				reinterpret_cast<WindowContainer*>(create->lpCreateParams);

			if (!wndContainer)
			{
				WX2_LOG_CRITICAL("windows_containerはnullptrです。");
				exit(EXIT_FAILURE);
			}

			// ウィンドウプロシージャを差し替え
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wndContainer));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));

			// 動的なウィンドウプロシージャに処理を委譲する
			return wndContainer->WindowProcedure(hwnd, msg, wp, lp);
		}
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
		}
	}
}