/*********************************************************************
 * @file	window_container.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	複数のウィンドウとメッセージの管理
 ********************************************************************/
#pragma once
#include "window.h"

namespace wx2
{
	class Window;

	/**
	 * @brief ウィンドウを格納するコンテナクラス
	 */
	class WindowContainer final
	{
	private:
		using WindowPtr = std::shared_ptr<Window>;

		//! ウィンドウ設定の保存先パス
		static constexpr const char* PROPERTY_FILE_NAME = "window.json";

	public:
		/**
		 * @brief ウィンドウ設定のデシリアライズを行う
		 */
		WindowContainer();

		/**
		 * @brief ウィンドウ設定のシリアライズを行う
		 */
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
		static void ProcessMessages(const std::function<bool()>& process);

		/**
		 * @brief アプリケーションの全てのウィンドウ共通のウィンドウプロシージャ
		 */
		static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	private:
		/**
		 * @brief ウィンドウ設定をシリアライズ化してファイルに保存する
		 */
		void Serialize();

		/**
		 * @brief ウィンドウ設定をシリアライズ化してファイルに保存する
		 */
		void Deserialize();

		//! ウィンドウポインタの連想配列
		std::unordered_map<
			std::string, 
			WindowPtr, 
			StringHash,
			std::equal_to<>> windows_;

		//! ウィンドウ設定の連想配列
		std::unordered_map<
			std::string, 
			WindowProperty, 
			StringHash,
			std::equal_to<>> windowProps_;
	};
}