/*********************************************************************
 * @file   vertex_type.h
 * @author Tomomi Murakami
 * @date   2021/11/26 18:51
 * @brief  ’¸“_Œ^’è‹`
 ********************************************************************/
#pragma once
#include <WX2Mathematics.h>

namespace wx2
{
	struct Vertex3D
	{
		Float3 position{};
	};

	struct ModelVertex
	{
		Float3 position{};
		Float2 texcoord{};
		Float3 normal{};
		Float3 tangent{};
		Float3 binormal{};
	};
}
