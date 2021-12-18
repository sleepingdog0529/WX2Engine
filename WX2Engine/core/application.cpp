#include "application.h"
#include <DirectXTK/GeometricPrimitive.h>
#include <sol/sol.hpp>

namespace wx2
{
	Application::Application() noexcept
	{
		WX2_ASSERT_MSG(!instantiated_, "�A�v���P�[�V�����͊��ɃC���X�^���X������Ă��܂��B");
		instantiated_ = true;

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

		instantiated_ = false;
	}

	int Application::Run(const float fps) noexcept
	{
		try
		{
			WX2_LOG_TRACE("�A�v���P�[�V�������s�J�n");

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

			// ���͂̏�����
			bool res = input_.Initialize(mainWindow_->GetHandle());
			WX2_RUNTIME_ERROR_IF_FAILED(res, "���͂̏������ŃG���[�������������߃A�v���P�[�V�������I�����܂��B");

			// �O���t�B�b�N�X�̏�����
			res = graphics_.Initialize(
				mainWindow_->GetHandle(),
				mainWindow_->GetWindowPropertyPtr(),
				true);
			WX2_RUNTIME_ERROR_IF_FAILED(res, "�O���t�B�b�N�X�̏������ŃG���[�������������߃A�v���P�[�V�������I�����܂��B");

			// �������Z�̏�����
			res = physics_.Initialize(0);
			WX2_RUNTIME_ERROR_IF_FAILED(res, "�������Z�̏������ŃG���[�������������߃A�v���P�[�V�������I�����܂��B");

			// �A�v���P�[�V�����J�n���̏������Ăяo��
			Start();

			// true�ŃA�v���P�[�V�����I��
			bool terminate = false;

			// FPS�v���p
			UINT frame = 0;	// 1�b�Ԃ̃t���[���J�E���^
			UINT measuredFps = 0;		// ����FPS
			UINT measuredAvgFps = 0;	// ��������FPS
			std::deque<int> fpsRecords{};	// FPS�L�^�p�f�L���[
			Timer<> frameTimer{};	// �t���[���v���p�^�C�}�[
			Timer<> fpsTimer{};		// FPS�v���p�^�C�}�[

			// ���C�����[�v
			while (!terminate)
			{
				// �Ō�̍X�V����̎��Ԍo��
				const auto elapced = frameTimer.ElapcedTime();

				// 1�t���[���̎��Ԃ��o�߂���܂ŃE�B���h�E���b�Z�[�W������
				if (elapced < 1000 / fps)
				{
					terminate = !windowContainer_.ProcessMessage();
				}
				else
				{
					// �t���[���v���^�C�}�[�����X�^�[�g
					frameTimer.Start();
					++frame;

					// 1�b�o�߂�����FPS���L�^����
					if (fpsTimer.ElapcedTime() >= 1000)
					{
						// FPS�v���^�C�}�[�����X�^�[�g
						fpsTimer.Start();

						// 1���ԕ���FPS���f�L���[�ɋL�^
						fpsRecords.push_back(frame);
						if (fpsRecords.size() > 3600)
						{
							fpsRecords.pop_front();
						}

						// FPS�����߂�
						measuredFps = frame;

						// FPS�̋L�^�̕��ς��畽��FPS�����߂�
						measuredAvgFps = std::reduce(fpsRecords.begin(), fpsRecords.end(), 0);
						measuredAvgFps /= static_cast<UINT>(fpsRecords.size());

						// �E�B���h�E�̃^�C�g����FPS�ɐݒ�
						mainWindow_->SetTitle(std::format("FPS: {} AVG: {}", measuredFps, measuredAvgFps));

						frame = 0;
					}

					// �t���[���̌o�ߎ��Ԃ����߂�
					const float deltaTime = static_cast<float>(elapced) / 1000.0f;

					// �A�v���P�[�V�����̕`�揈�����Ăяo��
					graphics_.DrawBegin();
					Draw(deltaTime);
					graphics_.DrawEnd();

					// ���͂̍X�V
					input_.Update();
					physics_.Step(deltaTime);

					// �A�v���P�[�V�����̍X�V���Ăяo��
					terminate = !Update(deltaTime);
				}
			}

			// ����I��
			return EXIT_SUCCESS;
		}
		catch (const RuntimeError& exception)
		{
			// �G���[�����O�o�͂��ăA�v���P�[�V�����I��
			WX2_LOG_ERROR(exception.what());
			return EXIT_FAILURE;
		}
		catch (const COMException& exception)
		{
			// COM�G���[�����O�o�͂��ăA�v���P�[�V�����I��
			WX2_LOG_ERROR(exception.what());
			return EXIT_FAILURE;
		}
	}
}