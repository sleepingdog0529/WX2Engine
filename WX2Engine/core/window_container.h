/*********************************************************************
 * @file	window_container.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	複数のウィンドウとメッセージの管理
*********************************************************************/
#pragma once
#include "window.h"
#include "window_property.h"

namespace wx2
{
	class Window;

	/**
	 * @brief ウィンドウを格納するコンテナクラス
	*/
	class WindowContainer
	{
	private:
		using WindowPropertyPtr = std::shared_ptr<WindowProperty>;
		using WindowPtr = std::shared_ptr<Window>;

		static constexpr const char* WINDOW_PROPERTY_PATH_ = ".\\window_property.json";

	public:
		/*! ウィンドウ設定をファイルからロードする */
		WindowContainer();

		/*! ウィンドウ設定をファイルにセーブする */
		virtual ~WindowContainer();

		WX2_DISALLOW_COPY_AND_MOVE(WindowContainer);

		/**
		* @brief     ウィンドウを生成する
		* @param[in] name 登録名
		* @param[in] defaultProp シリアライズ済みのデータが無い場合に使用するウィンドウ設定
		* @return    WindowPtr 作成したウィンドウのポインタ
		*/
		WindowPtr Create(const std::string& name, const WindowProperty& defaultProp);

		/**
		* @brief     メッセージを処理しつつ合間に更新処理を呼び出す
		* @param[in] process アプリケーションの更新関数
		*/
		void ProcessMessages(std::function<bool()> process);

		/**
		* @brief		アプリケーションの全てのウィンドウ共通のウィンドウプロシージャ
		* @param[in]	ウィンドウハンドル
		* @param[in]	メッセージ識別子
		* @param[in]	一つ目のパラメータ
		* @param[in]	二つ目のパラメータ
		* @return		メッセージの処理結果
		*/
		LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	private:
		/**
		* @brief	ウィンドウ設定をシリアライズ化してファイルに保存する
		*/
		void Serialize();

		/**
		* @brief	ウィンドウ設定をシリアライズ化してファイルに保存する
		*/
		void Deserialize();

		std::unordered_map<std::string, WindowPtr> windows_; //! ウィンドウポインタのの連想配列
		std::unordered_map<std::string, WindowPropertyPtr> windowProps_; //! ウィンドウ設定の連想配列
	};
}