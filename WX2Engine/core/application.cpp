#include "application.h"

namespace wx2
{
	Application::Application()
	{
		// ロケールの設定
		std::setlocale(LC_CTYPE, ".UTF-8");

		WX2_LOG_TRACE("アプリケーション初期化開始");

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
		wndProp.fullscreen = false;

		// メインウィンドウ生成
		auto mainWnd = windowContainer_.Create("MainWindow", wndProp);

		input_.Initialize(mainWnd->GetHandle());
	}

	Application::~Application()
	{
		WX2_LOG_TRACE("アプリケーション終了処理開始");
	}

	void Application::Run()
	{
		WX2_LOG_TRACE("アプリケーション実行開始");

		windowContainer_.ProcessMessages([this]()
		{
			input_.Update();

			return true;
		});
	}
}