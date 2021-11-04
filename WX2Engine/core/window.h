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

	class Window
	{
	public:
		/**
		 * @brief	  ウィンドウ生成
		 * @param[in] container 格納先コンテナ
		 * @param[in] window_prop ウィンドウ設定
		 */
		Window(WindowContainer* container, const WindowProperty& windowProp);
		/**
		 * @brief ウィンドウを閉じる
		 */
		virtual ~Window();

		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

		WX2_DISALLOW_COPY_AND_MOVE(Window);

		/**
		 * @return ウィンドウハンドル
		 */
		HWND GetHandle() const { return hwnd_; }

		/**
		 * @brief  
		 * @return ウィンドウ情報を取得
		 */
		const WindowProperty& GetWindowProperty() const { return windowProp_; }

		/**
		 * @brief     ウィンドウのフルスクリーン状態を設定する
		 * @param[in] フルスクリーンにするか
		 */
		void SetFullscreen(bool fullscreen);

		/**
		 * @brief     ウィンドウの最大化状態を設定する
		 * @param[in] 最大化するか
		 */
		void SetMaximize(bool maximaize);

	private:
		WindowContainer* container_;	//! ウィンドウコンテナ
		HWND hwnd_;						//! ウィンドウハンドル
		std::string className_;			//! ウィンドウクラス名
		WindowProperty windowProp_;		//! ウィンドウ情報
	};
}