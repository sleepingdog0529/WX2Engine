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

		//res = physics_.Initialize(4);
		//if (!res)
		//{
		//	WX2_LOG_ERROR("物理演算のエラーが発生したためアプリケーションを終了します。");
		//	return EXIT_FAILURE;
		//}

		WX2_LOG_TRACE("アプリケーション実行開始");

		// メインループ
		WindowContainer::ProcessMessages([this]() { return Update(); });

		return EXIT_SUCCESS;
	}

	bool Application::Update() noexcept
	{
		using namespace std::chrono_literals;

		input_.Update();

		const auto& keyboard = input_.GetKeyboard();

		// 前回の描画から1/60秒経過していたら描画する
		if (const auto elapced = timer_.ElapcedTime(); elapced >= 1s / 60)
		{
			timer_.Start();
			Draw();
		}

		// ESCキーが押されていたらアプリケーション終了
		return !keyboard.IsPressed(Keyboard::Escape);
	}

	void Application::Draw() noexcept
	{
		namespace DX = DirectX;

		constexpr auto pi = static_cast<float>(std::numbers::pi);

		const auto& device = graphics_.GetDevice().GetDevice();
		const auto& deviceContext = graphics_.GetDevice().GetDeviceContext();

		static float rad = 0.0f;
		rad = std::fmodf(rad + 0.0001f, pi * 2.0f);

		const float clearColor[4] = { std::cosf(rad) * 0.5f + 0.5f, 0.0f, 1.0f, 1.0f};
		deviceContext->ClearRenderTargetView(graphics_.GetRenderTargetView(), clearColor);
		deviceContext->RSSetViewports(1, graphics_.GetViewport());
		
		graphics_.GetVertexShader().Bind();
		graphics_.GetPixelShader().Bind();

		const WindowProperty wndProp = mainWindow_->GetWindowProperty();

		auto& constantBuffer = graphics_.GetConstantBufferWVP();
		auto* data = constantBuffer.GetData();
		data->world = DX::XMMatrixIdentity();
		data->view = DX::XMMatrixLookAtLH(
			DX::XMVECTORF32{ 0.0f, 0.0f, -1.0f },
			DX::XMVectorZero(),
			DX::XMVECTORF32{ 0.0f, 1.0f, 0.0f });
		data->projection = DX::XMMatrixPerspectiveFovLH(
			pi,
			static_cast<float>(wndProp.width) / static_cast<float>(wndProp.height),
			0.1f,
			1000.0f);
		constantBuffer.ApplyChange();
		constantBuffer.VSBind(0);

		graphics_.GetVertexBuffer().Bind();
		const auto& indexBuffer = graphics_.GetIndexBuffer();
		indexBuffer.Bind();

		deviceContext->DrawIndexed(indexBuffer.NumIndices(), 0, 0);

		auto* swapChain = graphics_.GetSwapChain();
		swapChain->Present(0, NULL);
	}
}
