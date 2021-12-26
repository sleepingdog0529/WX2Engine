/*********************************************************************
 * @file   texture_type.h
 * @author Tomomi Murakami
 * @date   2021/12/02 15:10
 * @brief  �e�N�X�`���̎��
 ********************************************************************/
#pragma once

namespace wx2
{
	enum class TextureType
	{
		Unknown,
		Diffuse,
		Specular,
		Ambient,
		Normals,
	};

	enum class TextureStorageType
	{
		Unknown,
		EmbeddedIndexCompressed,
		EmbeddedIndexNonCompressed,
		EmbeddedCompressed,
		EmbeddedNonCompressed,
		Disk
	};
}