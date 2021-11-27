#include "window.h"

namespace wx2
{
	Window::Window(WindowContainer* container, WindowProperty windowProp) noexcept :
		container_(container),
		windowProp_(std::move(windowProp))
	{
		WX2_ASSERT(windowProp_.width > 0);
		WX2_ASSERT(windowProp_.height > 0);

		// ランダムにユニークIDを生成
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::stringstream uuidStream;
		uuidStream << uuid;
		className_ = std::format("wx2eg-{}", uuidStream.str());

		// インスタンスハンドル取得
		const HINSTANCE hinst = GetModuleHandle(nullptr);

		// ウィンドウクラスを登録
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = HandleMessageSetup;
		wcex.hInstance = hinst;
		wcex.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wcex.lpszClassName = className_.c_str();
		if (!RegisterClassEx(&wcex)) [[unlikely]]
		{
			WX2_LOG_ERROR("ウィンドウクラスの登録に失敗しました。エラーコード: {}", GetLastError());
			exit(EXIT_FAILURE);
		}

		// ウィンドウを作成
		hwnd_ = CreateWindowEx(
			windowProp_.ex_style,
			className_.c_str(),
			windowProp_.title.c_str(),
			windowProp_.style,
			windowProp_.x,
			windowProp_.y,
			windowProp_.width,
			windowProp_.height,
			nullptr,
			nullptr,
			hinst,
			this
		);
		if (!hwnd_) [[unlikely]]
		{
			WX2_LOG_ERROR("ウィンドウの作成に失敗しました。エラーコード: {}", GetLastError());
			exit(EXIT_FAILURE);
		}

		// ウィンドウを可視化、更新
		UpdateWindow(hwnd_);

		SetFullscreen(windowProp_.fullscreen);
	}

	Window::~Window() noexcept
	{
		// ウィンドウを登録解除し、破棄する
		if (hwnd_) [[likely]]
		{
			UnregisterClass(className_.c_str(), GetModuleHandle(nullptr));
			DestroyWindow(hwnd_);
		}
	}

	LRESULT CALLBACK Window::HandleMessageRedirect(const HWND hwnd, const UINT msg, const WPARAM wp, const LPARAM lp) noexcept
	{
		// パラメータからウィンドウコンテナにキャストする
		auto* const window = std::bit_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (!window) [[unlikely]]
		{
			WX2_LOG_CRITICAL("ウィンドウクラスがパラメータから取得できませんでした。");
			exit(EXIT_FAILURE);
		}

		const WindowContainer* container = window->container_;

		switch (msg)
		{
			case WM_MOVING: 
				window->OnMoving(wp, lp); 
				return TRUE;

			case WM_SIZING: 
				window->OnSizing(wp, lp); 
				return TRUE;

			case WM_SYSCOMMAND:
				if (wp == SC_MAXIMIZE ||
					wp == SC_RESTORE)
				{
					window->OnDisplayModeChanged(wp, lp);
					return 0;
				}
				break;
				
			case WM_KEYDOWN:
				window->OnKeyDown(wp, lp);
				return 0;

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

			case WM_CLOSE:
				DestroyWindow(hwnd);
				return 0;

			default:
				break;
		}
		container->WindowProcedure(hwnd, msg, wp, lp);
	}

	LRESULT CALLBACK Window::HandleMessageSetup(const HWND hwnd, const  UINT msg, const WPARAM wp, const LPARAM lp) noexcept
	{
		// パラメータからウィンドウコンテナにキャストする
		const CREATESTRUCTW* const create = std::bit_cast<CREATESTRUCTW*>(lp);
		auto* const window = std::bit_cast<Window*>(create->lpCreateParams);

		if(msg == WM_NCCREATE)
		{
			if (!window) [[unlikely]]
			{
				WX2_LOG_CRITICAL("windows_containerはnullptrです。");
				exit(EXIT_FAILURE);
			}

			// ウィンドウプロシージャを差し替え
			SetWindowLongPtr(hwnd, GWLP_USERDATA, std::bit_cast<LONG_PTR>(window));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));

			// 動的なウィンドウプロシージャに処理を委譲する
			return window->container_->WindowProcedure(hwnd, msg, wp, lp);
		}

		return DefWindowProc(hwnd, msg, wp, lp);
	}

	void Window::SetFullscreen(const bool fullscreen) noexcept
	{
		windowProp_.fullscreen = fullscreen;

		if (windowProp_.fullscreen)
		{
			// ウィンドウスタイルからフルスクリーンに
			// 不要なフラグを取り除いて適応
			SetWindowLong(
				hwnd_, 
				GWL_STYLE,
				~(WS_CAPTION | WS_THICKFRAME) & windowProp_.style);
			SetWindowLong(
				hwnd_, 
				GWL_EXSTYLE,
				~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE) & windowProp_.ex_style);

			// ウィンドウサイズをモニターサイズと同じにする
			MONITORINFO mi{};
			mi.cbSize = sizeof(mi);
			GetMonitorInfo(MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST), &mi);
			SetWindowPos(
				hwnd_, 
				nullptr, 
				mi.rcMonitor.left, 
				mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		}
		else
		{
			// ウィンドウスタイルを再設定
			SetWindowLong(hwnd_, GWL_STYLE, static_cast<LONG>(windowProp_.style));
			SetWindowLong(hwnd_, GWL_EXSTYLE, static_cast<LONG>(windowProp_.ex_style));

			// ウィンドウサイズを復元
			SetWindowPos(
				hwnd_,
				nullptr,
				windowProp_.x,
				windowProp_.y,
				windowProp_.width,
				windowProp_.height,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

			// フルスクリーン前に最大化されていたら再度最大化
			if (windowProp_.maximized)
			{
				SetMaximize(true);
			}
		}
	}

	void Window::SetMaximize(const bool maximaize) noexcept
	{
		windowProp_.maximized = maximaize;
		SendMessage(hwnd_, WM_SYSCOMMAND, windowProp_.maximized ? SC_MAXIMIZE : SC_RESTORE, 0);
	}

	void Window::OnMoving([[maybe_unused]] const  WPARAM wp, const  LPARAM lp) noexcept
	{
		const auto rect = std::bit_cast<RECT*>(lp);
		windowProp_.x = static_cast<int>(rect->left);
		windowProp_.y = static_cast<int>(rect->top);
		windowProp_.maximized = false;
		windowProp_.fullscreen = false;
	}

	void Window::OnSizing([[maybe_unused]] const  WPARAM wp, const  LPARAM lp) noexcept
	{
		const auto rect = std::bit_cast<RECT*>(lp);
		windowProp_.x = static_cast<int>(rect->left);
		windowProp_.y = static_cast<int>(rect->top);
		windowProp_.width = static_cast<int>(rect->right - rect->left);
		windowProp_.height = static_cast<int>(rect->bottom - rect->top);
		windowProp_.maximized = false;
		windowProp_.fullscreen = false;
	}

	void Window::OnDisplayModeChanged(const WPARAM wp, [[maybe_unused]] const  LPARAM lp) noexcept
	{
		if      (wp == SC_MAXIMIZE) windowProp_.maximized = true;
		else if (wp == SC_RESTORE)  windowProp_.maximized = false;
	}

	void Window::OnKeyDown(const WPARAM wp, [[maybe_unused]] const  LPARAM lp) noexcept
	{
		if (wp == VK_F11) [[unlikely]]
		{
			SetFullscreen(!windowProp_.fullscreen);
		}
	}
}