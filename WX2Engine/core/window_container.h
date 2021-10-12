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
	class Window;

	/// <summary>
	/// 複数のウィンドウとメッセージの管理
	/// </summary>
	class WindowContainer
	{
	public:
		WindowContainer() = default;
		virtual ~WindowContainer() = default;

		WX2_DISALLOW_COPY_AND_MOVE(WindowContainer);

		/// <summary>
		/// ウィンドウを生成する
		/// </summary>
		/// <param name="name">登録名</param>
		/// <param name="windowProp">ウィンドウ設定</param>
		/// <returns>ウィンドウ</returns>
		std::shared_ptr<Window> Create(const std::string& name, const WindowProperty& windowProp);

		/// <summary>
		/// メッセージを処理しつつ合間に更新処理を呼び出す
		/// </summary>
		/// <param name="process">更新関数</param>
		void ProcessMessages(std::function<bool()> process);

		/// <summary>
		/// ウィンドウプロシージャ
		/// </summary>
		LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	private:
		std::unordered_map<std::string, std::shared_ptr<Window>> windows_;	// ウィンドウリスト
	};
}