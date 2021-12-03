/*********************************************************************
 * @file   texture_type.h
 * @author Tomomi Murakami
 * @date   2021/12/02 15:10
 * @brief  テクスチャの種類
 ********************************************************************/
#pragma once

namespace wx2::graphics
{
	enum class TextureType
	{
		Diffuse,
		Specular,
		Ambient,
		Normals,
	};
}