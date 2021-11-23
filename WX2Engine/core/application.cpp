#include "application.h"

namespace wx2
{
	Application::Application()
	{
		// ロケールの設定
		std::ios_base::sync_with_stdio(false);
		const std::locale defaultLoc("");
		std::locale::global(defaultLoc);
		const std::locale ctypeDefault(std::locale::classic(), defaultLoc, std::locale::ctype); 
		std::wcout.imbue(ctypeDefault);
		std::wcin.imbue(ctypeDefault);

		WX2_LOG_TRACE("アプリケーション初期化開始");
	}

	Application::~Application()
	{
		WX2_LOG_TRACE("アプリケーション終了処理開始");
	}

	void Application::Run()
	{
		// 画面サイズ取得
		const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// ウィンドウ情報を構築
		WindowProperty wndProp;
		wndProp.title = "WX2EG";
		wndProp.width = 1280;
		wndProp.height = 720;
		wndProp.x = (screenWidth - wndProp.width) / 2;
		wndProp.y = (screenHeight - wndProp.height) / 2;
		wndProp.style = WS_OVERLAPPEDWINDOW;
		wndProp.ex_style = WS_EX_WINDOWEDGE;
		wndProp.maximized = false;
		wndProp.fullscreen = false;

		// メインウィンドウ生成
		mainWindow_ = windowContainer_.Create("Main", wndProp);

		input_.Initialize(mainWindow_->GetHandle());

		bool res = graphics_.Initialize(
			mainWindow_->GetHandle(),
			mainWindow_->GetWindowProperty(),
			true);
		if (!res)
		{
			WX2_LOG_ERROR("グラフィックスエラーが発生したためアプリケーションを終了します。");
			return;
		}

		WX2_LOG_TRACE("アプリケーション実行開始");

		// メインループ
		windowContainer_.ProcessMessages([this]()
		{
			input_.Update();

			return !input_.IsPressed(Keyboard::Escape);
		});
	}
}