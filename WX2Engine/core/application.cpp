#include "application.h"

namespace wx2
{
	bool application::instantiated_ = false;

	application::application()
	{
		// ���P�[���̐ݒ�
		std::setlocale(LC_CTYPE, ".UTF-8");

		WX2_LOG_INFO("WX2Engine �������J�n...");

		// ��ʃT�C�Y�擾
		const int screen_width = GetSystemMetrics(SM_CXSCREEN);
		const int screen_height = GetSystemMetrics(SM_CYSCREEN);

		// �E�B���h�E�����\�z
		window_property wnd_prop;
		wnd_prop.title = "WX2EG";
		wnd_prop.width = 1280;
		wnd_prop.height = 720;
		wnd_prop.x = (screen_width - wnd_prop.width) / 2;
		wnd_prop.y = (screen_height - wnd_prop.height) / 2;
		wnd_prop.fullscreen = false;

		// ���C���E�B���h�E����
		window_container_.create("MainWindow", wnd_prop);
	}

	application::~application()
	{
	}

	void application::run()
	{
		window_container_.process_msgs([]()
		{
			return true;
		});
	}
}