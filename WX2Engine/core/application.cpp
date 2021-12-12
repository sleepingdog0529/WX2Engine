#include "application.h"
#include <DirectXTK/GeometricPrimitive.h>
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
		wndProp.exStyle = WS_EX_WINDOWEDGE;
		wndProp.maximized = false;
		wndProp.fullscreen = false;

		// ���C���E�B���h�E����
		mainWindow_ = windowContainer_.Create("Main", wndProp);

		input_.Initialize(mainWindow_->GetHandle());

		bool res = graphics_.Initialize(
			mainWindow_->GetHandle(),
			mainWindow_->GetWindowPropertyPtr(),
			true);
		if (!res)
		{
			WX2_LOG_ERROR("�O���t�B�b�N�X�̃G���[�������������߃A�v���P�[�V�������I�����܂��B");
			return EXIT_FAILURE;
		}

		WX2_LOG_TRACE("�A�v���P�[�V�������s�J�n");

		Start();

		// ���C�����[�v
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