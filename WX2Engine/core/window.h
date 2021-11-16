/*********************************************************************
 * @file	window.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	ウィンドウ作成
 ********************************************************************/
#pragma once
#include "window_container.h"
#include "window_property.h"

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
		Window(WindowContainer* container, const WindowProperty& windowProp);
		/**
		 * @brief ウィンドウを閉じる
		 */
		~Window();

		WX2_DISALLOW_COPY_AND_MOVE(Window);

		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageRedirect(const HWND hwnd, const UINT msg, const  WPARAM wp, const  LPARAM lp);
		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageSetup(const HWND hwnd, const  UINT msg, const WPARAM wp, const LPARAM lp);

		/**
		 * @return ウィンドウハンドル
		 */
		[[nodiscard]] HWND GetHandle() const
		{
			return hwnd_;
		}

		/**
		 * @brief 
		 * @return 
		 */
		[[nodiscard]] const WindowProperty& GetWindowProperty() const
		{
			return windowProp_;
		}

		/**
		 * @brief ウィンドウのフルスクリーン状態を設定する
		 * @param fullscreen フルスクリーンにするか
		 */
		void SetFullscreen(bool fullscreen);

		/**
		 * @brief ウィンドウの最大化状態を設定する
		 * @param maximaize 最大化するか
		 */
		void SetMaximize(const bool maximaize);

	private:
		//! ウィンドウ移動時のコールバック
		void OnMoving(WPARAM wp, LPARAM lp);

		//! ウィンドウサイズ変更時のコールバック
		void OnSizing(WPARAM wp, LPARAM lp);

		//! ウィンドウ表示モード変更時のコールバック
		void OnDisplayModeChanged(WPARAM wp, [[maybe_unused]] LPARAM lp);

		//! キー押下時
		void OnKeyDown(WPARAM wp, LPARAM lp);

		WindowContainer* container_;	//! ウィンドウコンテナ
		HWND hwnd_;						//! ウィンドウハンドル
		std::string className_;			//! ウィンドウクラス名
		WindowProperty windowProp_;		//! ウィンドウ情報
	};
}