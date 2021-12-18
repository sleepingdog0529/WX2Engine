#include "application.h"
#include <DirectXTK/GeometricPrimitive.h>
#include <sol/sol.hpp>

namespace wx2
{
	Application::Application() noexcept
	{
		WX2_ASSERT_MSG(!instantiated_, "アプリケーションは既にインスタンス化されています。");
		instantiated_ = true;

		// ロケールの設定
		std::ios_base::sync_with_stdio(false);
		const std::locale defaultLoc("");
		std::locale::global(defaultLoc);
		const std::locale ctypeDefault(std::locale::classic(), defaultLoc, std::locale::ctype);
		std::wcout.imbue(ctypeDefault);
		std::wcin.imbue(ctypeDefault);

		//physics::Physics physx;
		//physx.Initialize(16);

		WX2_LOG_TRACE("アプリケーション初期化開始");
	}

	Application::~Application() noexcept
	{
		WX2_LOG_TRACE("アプリケーション終了処理開始");

		instantiated_ = false;
	}

	int Application::Run() noexcept
	{
		try
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

			// 入力の初期化
			input_.Initialize(mainWindow_->GetHandle());

			// グラフィックスの初期化
			const bool res = graphics_.Initialize(
				mainWindow_->GetHandle(),
				mainWindow_->GetWindowPropertyPtr(),
				true);
			if (!res)
			{
				WX2_LOG_ERROR("グラフィックスのエラーが発生したためアプリケーションを終了します。");
				return EXIT_FAILURE;
			}

			WX2_LOG_TRACE("アプリケーション実行開始");

			// アプリケーション開始時の処理を呼び出す
			Start();

			// trueでアプリケーション終了
			bool terminate = false;

			// FPS計測用
			UINT frame = 0;	// 1秒間のフレームカウンタ
			UINT fps = 0;	// FPS
			UINT avg = 0;	// 平均FPS
			std::deque<int> fpsRecords{};	// FPS記録用デキュー
			Timer<> frameTimer{};	// フレーム計測用タイマー
			Timer<> fpsTimer{};		// FPS計測用タイマー

			// メインループ
			while (!terminate)
			{
				// 最後の更新からの時間経過
				const auto elapced = frameTimer.ElapcedTime();

				// 1フレームの時間が経過するまでウィンドウメッセージを処理
				if (elapced < 1000 / 120.0f)
				{
					terminate = !windowContainer_.ProcessMessage();
				}
				else
				{
					// フレーム計測タイマーをリスタート
					frameTimer.Start();
					++frame;

					// 1秒経過したらFPSを記録する
					if (fpsTimer.ElapcedTime() >= 1000)
					{
						// FPS計測タイマーをリスタート
						fpsTimer.Start();

						// 1時間分のFPSをデキューに記録
						fpsRecords.push_back(frame);
						if (fpsRecords.size() > 3600)
						{
							fpsRecords.pop_front();
						}

						// FPSを求める
						fps = frame;

						// FPSの記録の平均から平均FPSを求める
						avg = std::reduce(fpsRecords.begin(), fpsRecords.end(), 0);
						avg /= static_cast<UINT>(fpsRecords.size());

						// ウィンドウのタイトルをFPSに設定
						mainWindow_->SetTitle(std::format("FPS: {} AVG: {}", fps, avg));

						frame = 0;
					}

					// フレームの経過時間を求める
					const float deltaTime = static_cast<float>(elapced) / 120.0f;

					// アプリケーションの描画処理を呼び出す
					graphics_.DrawBegin();
					Draw(deltaTime);
					graphics_.DrawEnd();

					// 入力の更新
					input_.Update();

					// アプリケーションの更新を呼び出す
					terminate = !Update(deltaTime);
				}
			}

			// 正常終了
			return EXIT_SUCCESS;
		}
		catch (const RuntimeError& exception)
		{
			// エラーをログ出力してアプリケーション終了
			WX2_LOG_ERROR(exception.what());
			return EXIT_FAILURE;
		}
		catch (const COMException& exception)
		{
			// COMエラーをログ出力してアプリケーション終了
			WX2_LOG_ERROR(exception.what());
			return EXIT_FAILURE;
		}
	}
}