/*********************************************************************
 * @file	application.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:27
 * @brief	アプリケーション基礎
 ********************************************************************/
#pragma once
#include <WX2Input.h>
#include <WX2Graphics.h>
#include "window_container.h"

// エントリポイントを前方宣言
#if !defined(NDEBUG)
int main(int argc, char** argv);
#endif

namespace wx2
{
	/**
	 * @brief アプリケーション基礎クラス クライアントはこれを継承してアプリケーションの動作を定義する
	 */
	class Application
	{
	private:
		// エントリポイントでのみ実行を許可
#if !defined(NDEBUG)
		friend int ::main(int, char**);
#else
		friend int ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
#endif
		using WindowPtr = std::shared_ptr<Window>;

	public:
		//! @brief  アプリケーション初期化
		Application() noexcept;

		//! @brief  アプリケーション終了
		virtual ~Application() noexcept;

		
		WX2_DISALLOW_COPY(Application);
		WX2_DISALLOW_MOVE(Application);

	protected:
		//! @brief アプリケーション開始時の処理
		virtual void Start() noexcept = 0;

		/**
		 * @brief  アプリケーション更新時の処理
		 * @param  deltaTime 前回の更新からの経過時間
		 * @return falseでアプリケーション終了
		 */
		virtual bool Update(const float deltaTime) noexcept = 0;

		/**
		 * @brief  アプリケーションの描画処理
		 * @param  deltaTime 前回の更新からの経過時間
		 */
		virtual void Draw(const float deltaTime) noexcept = 0;

		//! @brief  ImGuiの描画処理
		virtual void DrawImGui() noexcept = 0;
		
		Logger log_;	//! ログ出力
		WindowContainer windowContainer_;	//! ウィンドウの管理コンテナ
		Input input_;						//! 入力管理
		WindowPtr mainWindow_;	//! メインウィンドウ
		Graphics graphics_;		//! 描画管理

	private:
		//! @brief アプリケーションを実行する 
		int Run() noexcept;

		//! 既にインスタンス化されているか
		static inline bool instantiated_ = false;
	};

	//! クライアントが定義する
	std::unique_ptr<Application> CreateApp() noexcept;
}