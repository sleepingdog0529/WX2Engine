/*********************************************************************
 * @file   shader_type.h
 * @author Tomomi Murakami
 * @date   2021/12/22 01:53
 * @brief  シェーダーの種類
 ********************************************************************/
#pragma once

namespace wx2
{
	enum class ShaderType
	{
		Vertex = 0,
		Pixel,
		Geometry,
		Hull,
		Domain,
		Compute
	};
}
