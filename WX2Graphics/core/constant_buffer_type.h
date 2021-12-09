/*********************************************************************
 * @file   constant_buffer_type.h
 * @author Tomomi Murakami
 * @date   2021/11/25 22:58
 * @brief  定数バッファの型定義
 ********************************************************************/
#pragma once
#include <WX2Mathematics.h>

namespace wx2::graphics
{
	struct WVPMatrix
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};
}
