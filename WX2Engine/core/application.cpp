#include "application.h"
#include <DirectXTK/GeometricPrimitive.h>
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
		wndProp.exStyle = WS_EX_WINDOWEDGE;
		wndProp.maximized = false;
		wndProp.fullscreen = false;

		// メインウィンドウ生成
		mainWindow_ = windowContainer_.Create("Main", wndProp);

		input_.Initialize(mainWindow_->GetHandle());

		bool res = graphics_.Initialize(
			mainWindow_->GetHandle(),
			mainWindow_->GetWindowPropertyPtr(),
			true);
		if (!res)
		{
			WX2_LOG_ERROR("グラフィックスのエラーが発生したためアプリケーションを終了します。");
			return EXIT_FAILURE;
		}

		WX2_LOG_TRACE("アプリケーション実行開始");

		Start();

		// メインループ
		bool terminate = false;
		UINT frame = 0;
		UINT fps = 0;
		UINT avg = 0;
		std::deque<int> fpsRecords{};
		Timer<> fpsTimer{};

		while (!terminate)
		{
			const auto elapced = frameTimer_.ElapcedTime();

			if (elapced < 1000 / 120.0f)
			{
				terminate = !windowContainer_.ProcessMessage();
			}
			else 
			{
				frameTimer_.Start();
				++frame;

				if (fpsTimer.ElapcedTime() >= 1000)
				{
					fpsTimer.Start();

					fpsRecords.push_back(frame);
					if (fpsRecords.size() > 60)
					{
						fpsRecords.pop_front();
					}

					fps = frame;

					avg = std::reduce(fpsRecords.begin(), fpsRecords.end(), 0);
					avg /= static_cast<UINT>(fpsRecords.size());

					mainWindow_->SetTitle(std::format("FPS: {} AVG: {}", fps, avg));

					frame = 0;
				}

				const float deltaTime = static_cast<float>(elapced) / 120.0f;

				graphics_.DrawBegin();
				Draw(deltaTime);
				graphics_.DrawEnd();

				terminate = !Update(deltaTime);
			}
		}

		return EXIT_SUCCESS;
	}
}