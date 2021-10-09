/*********************************************************************
 * @file	window_property.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:19
 * @brief	�E�B���h�E���
*********************************************************************/
#pragma once

namespace wx2 
{
	/// <summary>
	/// �E�B���h�E���
	/// </summary>
	struct window_property
	{
		std::string title;
		int x, y;
		int width, height;
		bool fullscreen;
	};
}