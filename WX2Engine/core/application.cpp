#include "application.h"

namespace wx2
{
	Application::Application() noexcept
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

	Application::~Application() noexcept
	{
		WX2_LOG_TRACE("アプリケーション終了処理開始");
	}

	int Application::Run() noexcept
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
			WX2_LOG_ERROR("グラフィックスのエラーが発生したためアプリケーションを終了します。");
			return EXIT_FAILURE;
		}

		res = physics_.Initialize(4);
		if (!res)
		{
			WX2_LOG_ERROR("物理演算のエラーが発生したためアプリケーションを終了します。");
			return EXIT_FAILURE;
		}

		WX2_LOG_TRACE("アプリケーション実行開始");

		// メインループ
		WindowContainer::ProcessMessages([this]() { return Update(); });

		return EXIT_SUCCESS;
	}

	bool Application::Update() noexcept
	{
		input_.Update();

		const auto& keyboard = input_.GetKeyboard();

		Draw();

		return !keyboard.IsPressed(Keyboard::Escape);
	}

	void Application::Draw() noexcept
	{
		const auto& devices = graphics_.GetDevice();
	}
}
