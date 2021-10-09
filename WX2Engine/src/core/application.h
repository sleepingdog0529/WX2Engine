/*********************************************************************
 * @file	application.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:27
 * @brief	アプリケーション基礎
*********************************************************************/
#pragma once
#include "macro.h"
#include "log.h"
#include "window_container.h"

int main(int argc, char** argv);

namespace wx2
{
	/// <summary>
	/// <para>アプリケーション基礎クラス</para>
	/// <para>クライアントはこれを継承してアプリケーションの動作を定義する</para>
	/// </summary>
	class application
	{
	public:
		application();
		virtual ~application();

		WX2_DISALLOW_COPY_AND_MOVE(application);

	private:
		/// <summary>
		/// アプリケーションを実行する
		/// </summary>
		void run();

		// main関数でのみ実行を許可
		friend int ::main(int argc, char** argv);

		logger log_;
		window_container window_container_;

		static bool instantiated_;
	};

	// クライアントが定義する
	std::unique_ptr<application> create_app();
}