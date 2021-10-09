#include "application.h"

namespace wx2
{
	bool application::instantiated_ = false;

	application::application()
	{
		// ロケールの設定
		std::setlocale(LC_CTYPE, ".UTF-8");

		WX2_LOG_INFO("WX2Engine 初期化開始...");

		// 画面サイズ取得
		const int screen_width = GetSystemMetrics(SM_CXSCREEN);
		const int screen_height = GetSystemMetrics(SM_CYSCREEN);

		// ウィンドウ情報を構築
		window_property wnd_prop;
		wnd_prop.title = "WX2EG";
		wnd_prop.width = 1280;
		wnd_prop.height = 720;
		wnd_prop.x = (screen_width - wnd_prop.width) / 2;
		wnd_prop.y = (screen_height - wnd_prop.height) / 2;
		wnd_prop.fullscreen = false;

		// メインウィンドウ生成
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