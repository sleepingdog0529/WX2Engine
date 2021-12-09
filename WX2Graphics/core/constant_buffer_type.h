/*********************************************************************
 * @file   constant_buffer_type.h
 * @author Tomomi Murakami
 * @date   2021/11/25 22:58
 * @brief  定数バッファの型定義
 ********************************************************************/
#pragma once
#include <DirectXMath.h>

namespace wx2::graphics
{
	struct WVPMatrix
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
}
