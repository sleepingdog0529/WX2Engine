/*********************************************************************
 * @file   assimp_key_translater.h
 * @author Tomomi Murakami
 * @date   2021/12/27 01:02
 * @brief  Assimp�̃L�[�񋓑̂����O�̗񋓑̂ɕϊ�����
 ********************************************************************/
#pragma once
#include <boost/bimap.hpp>
#include <boost/assign.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <WX2Common.h>
#include "texture_type.h"

namespace wx2
{
	namespace detail
	{
		using TextureTypeTranslate = boost::bimap<aiTextureType, TextureType>;

		static inline const TextureTypeTranslate TEXTURE_TYPE_TRANSLATOR =
			boost::assign::list_of<TextureTypeTranslate::relation>
			(aiTextureType::aiTextureType_DIFFUSE , TextureType::Diffuse)
			(aiTextureType::aiTextureType_SPECULAR, TextureType::Specular)
			(aiTextureType::aiTextureType_AMBIENT , TextureType::Ambient)
			(aiTextureType::aiTextureType_NORMALS , TextureType::Normals);
	}

	inline TextureType ToTextureType(const aiTextureType aiTexType)
	{
		try
		{ return detail::TEXTURE_TYPE_TRANSLATOR.left.at(aiTexType); }
		catch (...)
		{ return TextureType::Unknown; }
	}

	inline TextureStorageType ParseTextureStorageType(
		const aiScene* aiScene,
		const aiMaterial* aiMaterial,
		const UINT index,
		const aiTextureType aiTextureType) noexcept
	{
		if (aiMaterial->GetTextureCount(aiTextureType) == 0)
		{
			return TextureStorageType::Unknown;
		}

		aiString path;
		aiMaterial->GetTexture(aiTextureType, index, &path);
		std::string_view texturePath(path.C_Str());

		if (texturePath[0] == '*')
		{
			if (aiScene->mTextures[0]->mHeight == 0)
			{
				return TextureStorageType::EmbeddedIndexCompressed;
			}
			else
			{
				WX2_LOG_ERROR("�C���f�b�N�X�t���񈳏k�e�N�X�`���͓ǂݍ��ނ��Ƃ��o���܂���B");
				return TextureStorageType::EmbeddedIndexNonCompressed;
			}
		}
		else if (const auto aiTexture = aiScene->GetEmbeddedTexture(texturePath.data()))
		{
			if (aiTexture->mHeight == 0)
			{
				return TextureStorageType::EmbeddedCompressed;
			}
			else
			{
				WX2_LOG_ERROR("���ߍ��ݔ񈳏k�e�N�X�`���͓ǂݍ��ނ��Ƃ��o���܂���B");
				return TextureStorageType::EmbeddedNonCompressed;
			}
		}
		else if (texturePath.find('.') != std::string::npos)
		{
			return TextureStorageType::Disk;
		}

		return TextureStorageType::Unknown;
	}
}
