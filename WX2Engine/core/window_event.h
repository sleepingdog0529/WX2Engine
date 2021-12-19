/*********************************************************************
 * @file   window_event.h
 * @author Tomomi Murakami
 * @date   2021/12/19 23:18
 * @brief  ウィンドウのイベント
 ********************************************************************/
#pragma once

namespace wx2
{
	enum class WindowEvent
	{
		None = 0,
		Moving = WM_MOVING,
		Sizing = WM_SIZING,
	};
}
