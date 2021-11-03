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
		 * @brief 
		 */
		virtual ~Window();

		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
		//! ウィンドウのウィンドウプロシージャ
		static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

		WX2_DISALLOW_COPY_AND_MOVE(Window);

		HWND GetHandle() const { return hwnd_; }
		const WindowProperty& GetWindowProperty() const { return windowProp_; }

		void SetFullscreen(bool fullscreen);
		void SetMaximize(bool maximaize);

	private:
		WindowContainer* container_;	// ウィンドウコンテナ
		HWND hwnd_;						// ウィンドハンドル
		std::string className_;		// クラス名
		WindowProperty windowProp_;	// ウィンドウ情報
	};
}