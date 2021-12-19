#include "window.h"

namespace wx2
{
	Window::Window(WindowContainer* container, WindowProperty windowProp) 
		: container_(container)
		, windowProp_(std::move(windowProp))
	{
		WX2_ASSERT(windowProp_.width > 0);
		WX2_ASSERT(windowProp_.height > 0);

		// ランダムにユニークUUIDを生成し、ウィンドウクラス名にする
		const boost::uuids::uuid uuid = boost::uuids::random_generator()();
		std::stringstream uuidStream;
		uuidStream << uuid;
		className_ = std::format("wx2eg-{}", uuidStream.str());

		// インスタンスハンドル取得
		const HINSTANCE hinst = GetModuleHandle(nullptr);

		// ウィンドウクラスを登録
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;	// サイズ変更時に再描画メッセージを発行
		wcex.lpfnWndProc = HandleMessageSetup;
		wcex.hInstance = hinst;
		wcex.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wcex.lpszClassName = className_.c_str();
		WX2_RUNTIME_ERROR_IF_FAILED(
			RegisterClassEx(&wcex), "ウィンドウクラスの登録に失敗しました。エラーコード: {}", GetLastError());

		// ウィンドウを作成
		hwnd_ = CreateWindowEx(
			windowProp_.exStyle,
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
			this);
		WX2_RUNTIME_ERROR_IF_FAILED(
			hwnd_, "ウィンドウの作成に失敗しました。エラーコード: {}", GetLastError());

		// ウィンドウを可視化、更新
		UpdateWindow(hwnd_);

		// フルスクリーン設定
		if (windowProp_.fullscreen)
		{
			SetFullscreen(true);
		}
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

	LRESULT CALLBACK Window::HandleMessageRedirect(const HWND hwnd, const UINT msg, const WPARAM wp, const LPARAM lp) 
	{
		// パラメータからウィンドウコンテナにキャストする
		auto* const window = std::bit_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		WX2_RUNTIME_ERROR_IF_FAILED(window, "ウィンドウクラスがパラメータから取得できませんでした。");

		switch (msg)
		{
		case WM_MOVING:	// ウィンドウ移動時
			window->OnMoving(wp, lp);
			break;

		case WM_SIZING: // ウィンドウサイズ変更時
			window->OnSizing(wp, lp);
			break;

		case WM_SYSCOMMAND:	// ウィンドウ最大化、通常化時
			if (wp == SC_MAXIMIZE ||
				wp == SC_RESTORE)
			{
				window->OnDisplayModeChanged(wp, lp);
				return 0;
			}
			break;

		case WM_SETTEXT:	// ウィンドウタイトル変更時
			window->OnTitleChanged(wp, lp);
			break;

		case WM_KEYDOWN:	// キー押下時
			window->OnKeyDown(wp, lp);
			break;

		case WM_DESTROY:	// ウィンドウを破棄したとき
			PostQuitMessage(0);
			return 0;

		case WM_CLOSE:	// ウィンドウを閉じたとき
			DestroyWindow(hwnd);
			return 0;

		default:
			break;
		}

		// 動的なウィンドウプロシージャに処理を委譲する
		return window->container_->WindowProcedure(hwnd, msg, wp, lp);
	}

	LRESULT CALLBACK Window::HandleMessageSetup(const HWND hwnd, const  UINT msg, const WPARAM wp, const LPARAM lp)
	{
		// パラメータからウィンドウコンテナにキャストする
		const CREATESTRUCTW* const create = std::bit_cast<CREATESTRUCTW*>(lp);
		auto* const window = std::bit_cast<Window*>(create->lpCreateParams);

		if (msg == WM_NCCREATE)
		{
			WX2_RUNTIME_ERROR_IF_FAILED(window, "ウィンドウクラスがパラメータから取得できませんでした。");

			// ウィンドウプロシージャを差し替え
			SetWindowLongPtr(hwnd, GWLP_USERDATA, std::bit_cast<LONG_PTR>(window));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));

			// 動的なウィンドウプロシージャに処理を委譲する
			return window->container_->WindowProcedure(hwnd, msg, wp, lp);
		}

		// デフォルトウィンドウプロシージャ
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	void Window::SetFullscreen(const bool fullscreen) noexcept
	{
		// フルスクリーンフラグ変更
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
				~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE) & windowProp_.exStyle);

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

			// 通常時のウィンドウ情報を記録
			recordWindowX_ = windowProp_.x;
			recordWindowY_ = windowProp_.y;
			recordWindowWidth_ = windowProp_.width;
			recordWindowHeight_ = windowProp_.height;

			// ウィンドウ情報の位置とサイズをディスプレイと同じにする
			windowProp_.x = mi.rcMonitor.left;
			windowProp_.y = mi.rcMonitor.top;
			windowProp_.width = mi.rcMonitor.right - mi.rcMonitor.left;
			windowProp_.height = mi.rcMonitor.bottom - mi.rcMonitor.top;
		}
		else
		{
			// ウィンドウスタイルを再設定
			SetWindowLong(hwnd_, GWL_STYLE, static_cast<LONG>(windowProp_.style));
			SetWindowLong(hwnd_, GWL_EXSTYLE, static_cast<LONG>(windowProp_.exStyle));

			// ウィンドウサイズを復元
			SetWindowPos(
				hwnd_,
				nullptr,
				recordWindowX_,
				recordWindowY_,
				recordWindowWidth_,
				recordWindowHeight_,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

			// ウィンドウ情報を記録した情報に戻す
			windowProp_.x = recordWindowX_;
			windowProp_.y = recordWindowY_;
			windowProp_.width = recordWindowWidth_;
			windowProp_.height = recordWindowHeight_;

			// フルスクリーン前に最大化されていたなら再度最大化
			if (windowProp_.maximized)
			{
				SetMaximize(true);
			}
		}
	}

	void Window::SetMaximize(const bool maximaize) noexcept
	{
		windowProp_.maximized = maximaize;

		// 最大化or通常化メッセージを発行
		SendMessage(hwnd_, WM_SYSCOMMAND, windowProp_.maximized ? SC_MAXIMIZE : SC_RESTORE, 0);
	}

	void Window::SetTitle(const std::string& title) noexcept
	{
		windowProp_.title = title;

		// ウィンドウタイトルを設定
		SetWindowText(hwnd_, title.c_str());
	}

	void Window::OnMoving([[maybe_unused]] const  WPARAM wp, const  LPARAM lp) noexcept
	{
		// パラメータからウィンドウ位置を取得
		const auto rect = std::bit_cast<RECT*>(lp);
		windowProp_.x = static_cast<int>(rect->left);
		windowProp_.y = static_cast<int>(rect->top);
		windowProp_.maximized = false;
		windowProp_.fullscreen = false;
	}

	void Window::OnSizing([[maybe_unused]] const  WPARAM wp, const  LPARAM lp) noexcept
	{
		// パラメータからウィンドウ位置とサイズを取得
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
		// パラメータから最大化状態を取得
		if (wp == SC_MAXIMIZE) windowProp_.maximized = true;
		else if (wp == SC_RESTORE)  windowProp_.maximized = false;
	}

	void Window::OnTitleChanged(const WPARAM wp, const LPARAM lp) noexcept
	{
		// パラメータからタイトル名を取得
		windowProp_.title = std::string(reinterpret_cast<char*>(lp));
	}

	void Window::OnKeyDown(const WPARAM wp, [[maybe_unused]] const  LPARAM lp) noexcept
	{
		// F11キーでウィンドウを最大化
		if (wp == VK_F11) [[unlikely]]
		{
			SetFullscreen(!windowProp_.fullscreen);
		}
	}
}