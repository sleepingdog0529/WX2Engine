#include "application.h"

namespace wx2
{
	Application::Application()
	{
		// ロケールの設定
		std::setlocale(LC_CTYPE, ".UTF-8");

		WX2_LOG_INFO("WX2Engine 初期化開始...");

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

		Input::Initialize();
		Mouse::Initialize(mainWnd->GetHandle());
	}

	Application::~Application()
	{
		Mouse::Finalize();
	}

	void Application::Run()
	{
		windowContainer_.ProcessMessages([]()
		{
			Mouse::Update();

			return !Mouse::IsReleased(Mouse::X2);
		});
	}
}