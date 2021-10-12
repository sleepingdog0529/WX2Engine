#include "application.h"

namespace wx2
{
	Application::Application()
	{
		// ���P�[���̐ݒ�
		std::setlocale(LC_CTYPE, ".UTF-8");

		WX2_LOG_INFO("WX2Engine �������J�n...");

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