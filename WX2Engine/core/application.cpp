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
		wndProp.fullscreen = false;

		// ���C���E�B���h�E����
		auto mainWnd = windowContainer_.Create("MainWindow", wndProp);

		input_.Initialize(mainWnd->GetHandle());
	}

	Application::~Application()
	{
		WX2_LOG_TRACE("�A�v���P�[�V�����I�������J�n");
	}

	void Application::Run()
	{
		WX2_LOG_TRACE("�A�v���P�[�V�������s�J�n");

		windowContainer_.ProcessMessages([this]()
		{
			input_.Update();

			return true;
		});
	}
}