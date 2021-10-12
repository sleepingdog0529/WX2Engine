/*********************************************************************
 * @file	window_property.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:19
 * @brief	ウィンドウ情報
*********************************************************************/
#pragma once

namespace wx2 
{
	/// <summary>
	/// ウィンドウ情報
	/// </summary>
	struct WindowProperty
	{
		std::string title;
		int x, y;
		int width, height;
		bool fullscreen;
	};
}