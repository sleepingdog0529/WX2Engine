/*********************************************************************
 * @file	window_container.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	複数のウィンドウとメッセージの管理
 ********************************************************************/
#pragma once
#include "window.h"
#include "window_event.h"

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
		using EventListenerType = EventListener<WindowEvent, HWND, WPARAM, LPARAM>;
		using EventCallbackType = std::function<void(HWND, WPARAM, LPARAM)>;

		//! ウィンドウ設定の保存先パス
		static constexpr const char* PROPERTY_FILE_NAME = "window.json";

	public:
		/**
		 * @brief ウィンドウ設定のデシリアライズを行う
		 */
		WindowContainer() noexcept;

		/**
		 * @brief ウィンドウ設定のシリアライズを行う
		 */
		virtual ~WindowContainer() noexcept;

		WX2_DISALLOW_COPY(WindowContainer);
		WX2_DISALLOW_MOVE(WindowContainer);

		/**
		 * @brief     ウィンドウを生成する
		 * @param[in] name 登録名
		 * @param[in] defaultProp シリアライズ済みのデータが無い場合に使用するウィンドウ設定
		 * @return    WindowPtr 作成したウィンドウのポインタ
		 */
		WindowPtr Create(const std::string& name, const WindowProperty& defaultProp);


		EventListenerType AppendCallback(
			WindowEvent event,
			const EventCallbackType& callback) noexcept;

		/**
		 * @brief  全てのウィンドウのメッセージを一つ処理する
		 * @return WM_QUITが投げられていなければtrue
		 */
		static bool ProcessMessage() noexcept;

		/**
		 * @brief アプリケーションの全てのウィンドウ共通のウィンドウプロシージャ
		 */
		LRESULT CALLBACK WindowProcedure(const HWND hwnd, const UINT msg, const WPARAM wp, const LPARAM lp) noexcept;

	private:
		/**
		 * @brief ウィンドウ設定をシリアライズ化してファイルに保存する
		 */
		void Serialize() noexcept;

		/**
		 * @brief ウィンドウ設定をシリアライズ化してファイルに保存する
		 */
		void Deserialize() noexcept;

		//! ウィンドウポインタの連想配列
		std::unordered_map<std::string, WindowPtr> windows_;

		//! ウィンドウ設定の連想配列
		std::unordered_map<std::string, WindowProperty> windowProps_;

		//! ウィンドウイベント発行
		EventDispatcher<WindowEvent, HWND, WPARAM, LPARAM> eventDispatcher_;
	};
}