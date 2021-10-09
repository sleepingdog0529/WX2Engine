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
	class window;

	/// <summary>
	/// 複数のウィンドウとメッセージの管理
	/// </summary>
	class window_container
	{
	public:
		window_container() = default;
		~window_container() = default;

		WX2_DISALLOW_COPY_AND_MOVE(window_container);

		/// <summary>
		/// ウィンドウを生成する
		/// </summary>
		/// <param name="name">登録名</param>
		/// <param name="window_prop">ウィンドウ設定</param>
		/// <returns>ウィンドウ</returns>
		std::shared_ptr<window> create(const std::string& name, const window_property& window_prop);

		/// <summary>
		/// メッセージを処理しつつ合間に更新処理を呼び出す
		/// </summary>
		/// <param name="process">更新関数</param>
		void process_msgs(std::function<bool()> process);

		/// <summary>
		/// ウィンドウプロシージャ
		/// </summary>
		LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	private:
		std::unordered_map<std::string, std::shared_ptr<window>> render_windows_;	// ウィンドウリスト
	};
}