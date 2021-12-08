#include "application.h"

namespace wx2
{
	Application::Application() noexcept
	{
		// ���P�[���̐ݒ�
		std::ios_base::sync_with_stdio(false);
		const std::locale defaultLoc("");
		std::locale::global(defaultLoc);
		const std::locale ctypeDefault(std::locale::classic(), defaultLoc, std::locale::ctype);
		std::wcout.imbue(ctypeDefault);
		std::wcin.imbue(ctypeDefault);

		WX2_LOG_TRACE("�A�v���P�[�V�����������J�n");
	}

	Application::~Application() noexcept
	{
		WX2_LOG_TRACE("�A�v���P�[�V�����I�������J�n");
	}

	int Application::Run() noexcept
	{
		// ��ʃT�C�Y�擾
		const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// �E�B���h�E�����\�z
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

		// ���C���E�B���h�E����
		mainWindow_ = windowContainer_.Create("Main", wndProp);

		input_.Initialize(mainWindow_->GetHandle());

		bool res = graphics_.Initialize(
			mainWindow_->GetHandle(),
			mainWindow_->GetWindowProperty(),
			true);
		if (!res)
		{
			WX2_LOG_ERROR("�O���t�B�b�N�X�̃G���[�������������߃A�v���P�[�V�������I�����܂��B");
			return EXIT_FAILURE;
		}

		WX2_LOG_TRACE("�A�v���P�[�V�������s�J�n");

		// ���C�����[�v
		WindowContainer::ProcessMessages([this]() { return Update(); });

		return EXIT_SUCCESS;
	}

	bool Application::Update() noexcept
	{
		using namespace std::chrono_literals;

		input_.Update();

		const auto& keyboard = input_.GetKeyboard();

		// �O��̕`�悩��1/60�b�o�߂��Ă�����`�悷��
		if (const auto elapced = timer_.ElapcedTime(); elapced >= 1s / 60)
		{
			timer_.Start();
			graphics_.RenderFrame();
		}

		// ESC�L�[��������Ă�����A�v���P�[�V�����I��
		return !keyboard.IsPressed(Keyboard::Escape);
	}

	void Application::Draw() noexcept
	{
		namespace DX = DirectX;

		constexpr auto pi = static_cast<float>(std::numbers::pi);

		const auto& devices = graphics_.GetDevice();
		const auto& dev = devices.GetDevice();
		const auto& devCon = devices.GetDeviceContext();

		graphics_.GetVertexShader().Bind();
		graphics_.GetPixelShader().Bind();

		//auto& constantBuffer = graphics_.GetConstantBufferWVP();
		//constantBuffer.data.world = DX::XMMatrixIdentity();
		//constantBuffer.data.view = DX::XMMatrixLookAtLH(
		//	DX::XMVECTORF32{ 0.0f, 0.0f, -1.0f },
		//	DX::XMVectorZero(),
		//	DX::XMVECTORF32{ 0.0f, 100.0f, 0.0f });
		//constantBuffer.ApplyChange();
		//constantBuffer.VSBind(0);

		graphics_.GetVertexBuffer().Bind();
		const auto& indexBuffer = graphics_.GetIndexBuffer();
		indexBuffer.Bind();

		devCon->DrawIndexed(indexBuffer.NumIndices(), 0, 0);
	}
}