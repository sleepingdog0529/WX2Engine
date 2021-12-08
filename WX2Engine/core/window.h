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
	class WindowContainer;

	class Window final
	{
	public:
		/**
		 * @brief ウィンドウ生成
		 * @param[in] container 格納先コンテナ
		 * @param[in] windowProp ウィンドウ設定
		 */
		Window(WindowContainer* container, WindowProperty windowProp) noexcept;

		/**
		 * @brief ウィンドウを閉じる
		 */
		~Window() noexcept;

		WX2_DISALLOW_COPY_AND_MOVE(Window);

		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageRedirect(const HWND hwnd, const UINT msg, const  WPARAM wp, const  LPARAM lp) noexcept;
		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageSetup(const HWND hwnd, const  UINT msg, const WPARAM wp, const LPARAM lp) noexcept;

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
		 * @brief ウィンドウのフルスクリーン状態を設定する
		 * @param fullscreen フルスクリーンにするか
		 */
		void SetFullscreen(bool fullscreen) noexcept;

		/**
		 * @brief ウィンドウの最大化状態を設定する
		 * @param maximaize 最大化するか
		 */
		void SetMaximize(const bool maximaize) noexcept;

	private:
		//! ウィンドウ移動時のコールバック
		void OnMoving(const WPARAM wp, const  LPARAM lp) noexcept;

		//! ウィンドウサイズ変更時のコールバック
		void OnSizing(const WPARAM wp, const LPARAM lp) noexcept;

		//! ウィンドウ表示モード変更時のコールバック
		void OnDisplayModeChanged(const WPARAM wp, const  LPARAM lp) noexcept;

		//! キー押下時のコールバック
		void OnKeyDown(const WPARAM wp, const LPARAM lp) noexcept;

		WindowContainer* container_;	//! ウィンドウコンテナ
		HWND hwnd_;						//! ウィンドウハンドル
		std::string className_;			//! ウィンドウクラス名
		WindowProperty windowProp_;		//! ウィンドウ情報
	};
}