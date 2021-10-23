/*********************************************************************
 * @file	application.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:27
 * @brief	アプリケーション基礎
*********************************************************************/
#pragma once
#include <wx2common.h>
#include <WX2Input.h>
#include "window_container.h"

int main(int argc, char** argv);

namespace wx2
{
	/// <summary>
	/// <para>アプリケーション基礎クラス</para>
	/// <para>クライアントはこれを継承してアプリケーションの動作を定義する</para>
	/// </summary>
	class Application
	{
	private:
		// エントリポイントでのみ実行を許可
#ifdef NDEBUG
		friend INT WINAPI ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
#else
		friend int ::main(int, char**);
#endif

	public:
		Application();
		virtual ~Application();

		WX2_DISALLOW_COPY_AND_MOVE(Application);

	private:
		/// <summary>
		/// アプリケーションを実行する
		/// </summary>
		void Run();

		Logger log_;
		WindowContainer windowContainer_;
		Input input_;
		std::shared_ptr<Window> mainWindow_;

		static inline bool instantiated_ = false;
	};

	// クライアントが定義する
	std::unique_ptr<Application> CreateApp();
}