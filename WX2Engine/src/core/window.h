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
	class window_container;

	class window
	{
	public:
		/// <summary>
		/// ウィンドウ生成
		/// </summary>
		/// <param name="container">格納先コンテナ</param>
		/// <param name="window_prop">ウィンドウ設定</param>
		window(window_container* container, const window_property& window_prop);
		virtual ~window();

		WX2_DISALLOW_COPY_AND_MOVE(window);

		/// <returns>ウィンドウハンドル</returns>
		HWND get_handle() const { return hwnd_; }
		/// <returns>ウィンドウ横幅</returns>
		int get_width() const { return window_prop_.width; }
		/// <returns>ウィンドウ縦幅</returns>
		int get_height() const { return window_prop_.height; }
		/// <returns>フルスクリーンか</returns>
		bool is_fullscreen() const { return window_prop_.fullscreen; }

	private:
		window_container* container_;	// ウィンドウコンテナ
		HWND hwnd_;						// ウィンドハンドル
		std::string class_name_;		// クラス名
		window_property window_prop_;	// ウィンドウ情報
	};
}