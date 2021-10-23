/*********************************************************************
 * @file	window.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:36
 * @brief	ウィンドウ作成
*********************************************************************/
#pragma once
#include "window_container.h"
#include "window_property.h"

namespace wx2 
{
	class WindowContainer;

	class Window
	{
	private:
		using WindowPropertyPtr = std::shared_ptr<WindowProperty>;

	public:
		/// <summary>
		/// ウィンドウ生成
		/// </summary>
		/// <param name="container">格納先コンテナ</param>
		/// <param name="window_prop">ウィンドウ設定</param>
		Window(WindowContainer* container, WindowPropertyPtr windowProp);
		virtual ~Window();

		static LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
		static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

		WX2_DISALLOW_COPY_AND_MOVE(Window);

		HWND GetHandle() const { return hwnd_; }
		WindowPropertyPtr GetWindowProperty() const { return windowProp_; }
		int GetWidth() const { return windowProp_->width; }
		int GetHeight() const { return windowProp_->height; }
		bool IsFullscreen() const { return windowProp_->fullscreen; }

	private:
		WindowContainer* container_;	// ウィンドウコンテナ
		HWND hwnd_;						// ウィンドハンドル
		std::string className_;		// クラス名
		WindowPropertyPtr windowProp_;	// ウィンドウ情報
	};
}