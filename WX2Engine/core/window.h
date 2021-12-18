/*********************************************************************
 * @file	window.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	ウィンドウ作成
 ********************************************************************/
#pragma once
#include "window_container.h"

namespace wx2
{
	// 前方宣言
	class WindowContainer;

	/**
	 * @brief  ウィンドウ
	 */
	class Window final
	{
	public:
		/**
		 * @brief ウィンドウ生成
		 * @param[in] container 格納先コンテナ
		 * @param[in] windowProp ウィンドウ設定
		 */
		Window(WindowContainer* container, WindowProperty windowProp);

		//! @brief ウィンドウを閉じる
		~Window() noexcept;

		WX2_DISALLOW_COPY(Window);
		WX2_DISALLOW_MOVE(Window);

		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageRedirect(const HWND hwnd, const UINT msg, const  WPARAM wp, const  LPARAM lp);
		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageSetup(const HWND hwnd, const  UINT msg, const WPARAM wp, const LPARAM lp);

		/**
		 * @brief  ウィンドウハンドルを取得する
		 * @return ウィンドウハンドル
		 */
		[[nodiscard]] HWND GetHandle() const noexcept
		{
			return hwnd_;
		}

		/**
		 * @brief  ウィンドウ情報を取得する
		 * @return ウィンドウ情報
		 */
		[[nodiscard]] const WindowProperty& GetWindowProperty() const noexcept
		{
			return windowProp_;
		}

		/**
		 * @brief  ウィンドウ情報ポインタを取得する
		 * @return ウィンドウ情報ポインタ
		 */
		[[nodiscard]] WindowProperty* GetWindowPropertyPtr() noexcept
		{
			return &windowProp_;
		}

		/**
		 * @brief ウィンドウのフルスクリーン状態を設定する
		 * @param fullscreen フルスクリーンにするか
		 */
		void SetFullscreen(bool fullscreen) noexcept;

		/**
		 * @brief ウィンドウの最大化状態を設定する
		 * @param maximaize 最大化するか
		 */
		void SetMaximize(const bool maximaize) noexcept;

		/**
		 * @brief  ウィンドウのタイトルを設定する
		 * @param  title 設定するタイトル名
		 */
		void SetTitle(const std::string& title) noexcept;

	private:
		//! ウィンドウ移動時のコールバック
		void OnMoving(const WPARAM wp, const  LPARAM lp) noexcept;

		//! ウィンドウサイズ変更時のコールバック
		void OnSizing(const WPARAM wp, const LPARAM lp) noexcept;

		//! ウィンドウ表示モード変更時のコールバック
		void OnDisplayModeChanged(const WPARAM wp, const LPARAM lp) noexcept;

		//! ウィンドウ名変更時のコールバック
		void OnTitleChanged(const WPARAM wp, const LPARAM lp) noexcept;

		//! キー押下時のコールバック
		void OnKeyDown(const WPARAM wp, const LPARAM lp) noexcept;

		WindowContainer* container_{};	//! ウィンドウコンテナ
		HWND hwnd_{};					//! ウィンドウハンドル
		std::string className_{};		//! ウィンドウクラス名
		WindowProperty windowProp_{};	//! ウィンドウ情報

		// フルスクリーンを解除するときに必要な情報を記憶しておくための変数
		int recordWindowX_{};			//! 通常時ウィンドウのX座標の記録
		int recordWindowY_{};			//! 通常時ウィンドウのY座標の記録
		int recordWindowWidth_{};		//! 通常時ウィンドウの幅の記録
		int recordWindowHeight_{};		//! 通常時ウィンドウの高さの記録
	};
}