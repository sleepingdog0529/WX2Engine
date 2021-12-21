/*********************************************************************
 * @file   texture_type.h
 * @author Tomomi Murakami
 * @date   2021/12/02 15:10
 * @brief  テクスチャの種類
 ********************************************************************/
#pragma once
#include <assimp/material.h>

namespace wx2
{
	enum class TextureType
	{
		Diffuse  = aiTextureType::aiTextureType_DIFFUSE,
		Specular = aiTextureType::aiTextureType_SPECULAR,
		Ambient  = aiTextureType::aiTextureType_AMBIENT,
		Normals  = aiTextureType::aiTextureType_NORMALS
	};
}
