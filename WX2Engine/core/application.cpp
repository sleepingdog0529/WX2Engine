#include "application.h"

namespace wx2
{
	Application::Application()
	{
		// ���P�[���̐ݒ�
		std::setlocale(LC_CTYPE, ".UTF-8");

		WX2_LOG_TRACE("�A�v���P�[�V�����������J�n");

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

		graphics_.Initialize(mainWindow_->GetHandle(), mainWindow_->GetWindowProperty(), true);
	}

	Application::~Application()
	{
		WX2_LOG_TRACE("�A�v���P�[�V�����I�������J�n");
	}

	void Application::Run()
	{
		WX2_LOG_TRACE("�A�v���P�[�V�������s�J�n");

		// ���C�����[�v
		windowContainer_.ProcessMessages([this]()
		{
			input_.Update();

			return !input_.IsPressed(Keyboard::Escape);
		});
	}
}