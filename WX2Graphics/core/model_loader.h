/*********************************************************************
 * @file   model_loader.h
 * @author Tomomi Murakami
 * @date   2021/11/29 16:07
 * @brief  ÉÇÉfÉãì«çû
 ********************************************************************/
#pragma once
#include <memory>
#include <filesystem>
#include <assimp/scene.h>
#include "model.h"
#include "model_loader.h"
#include "texture_type.h"

namespace wx2::graphics
{
	enum class TextureStorageType
	{
		Invalid,
		None,
		EmbeddedIndexCompressed,
		EmbeddedIndexNonCompressed,
		EmbeddedCompressed,
		EmbeddedNonCompressed,
		Disk
	};

	enum class MatKeyType
	{
		ColorDiffuse,
		ColorSpecular,
		ColorAmbient,
		Shininess,
		ShininessStrength
	};

	class ModelLoader
	{
	private:
		using ModelPtr = std::shared_ptr<Model>;
		using MatKeyTranslate = std::tuple<std::string, int, int>;

	public:
		ModelLoader() = default;
		~ModelLoader() = default;

		WX2_DISALLOW_COPY_AND_MOVE(ModelLoader);

		void Initialize(
			Device* devices,
			ConstantBuffer<WVPMatrix>* constantBufferWVP);

		Model Load(const std::filesystem::path& filePath);

	private:
		Mesh ProcessMesh(
			const aiMesh* aiMesh,
			const aiScene* aiScene,
			const std::filesystem::path& directory,
			const DirectX::XMMATRIX transformMatrix) const noexcept;

		Model ProcessNode(
			std::vector<Mesh>& meshes,
			const aiNode* aiNode,
			const aiScene* aiScene,
			const std::filesystem::path& directory,
			const DirectX::XMMATRIX parentTransformMatrix) noexcept;

		TextureStorageType DetermineTextureStorageType(const aiScene* aiScene, const aiMaterial* aiMaterial, UINT index, aiTextureType aiTextureType) const;

		std::map<TextureType, Texture> LoadMaterialTextures(
			const aiMaterial* aiMat,
			const aiScene* aiScene,
			const std::filesystem::path& directory) const;

		static inline const std::map<MatKeyType, MatKeyTranslate> MAT_KEY_TRANSLATOR = {
			{ MatKeyType::ColorSpecular		, { AI_MATKEY_COLOR_SPECULAR	} },
			{ MatKeyType::ColorDiffuse		, { AI_MATKEY_COLOR_DIFFUSE		} },
			{ MatKeyType::ColorAmbient		, { AI_MATKEY_COLOR_AMBIENT		} },
			{ MatKeyType::Shininess			, { AI_MATKEY_SHININESS			} },
			{ MatKeyType::ShininessStrength	, { AI_MATKEY_SHININESS			} } };

		static inline const std::map<aiTextureType, TextureType> TEXTURE_TYPE_TRANSLATOR = {
			{ aiTextureType::aiTextureType_DIFFUSE	, TextureType::Diffuse	},
			{ aiTextureType::aiTextureType_SPECULAR	, TextureType::Specular	},
			{ aiTextureType::aiTextureType_AMBIENT	, TextureType::Ambient	},
		};

		Device* devices_{};
		ConstantBuffer<WVPMatrix>* constantBufferWVP_;
	};
}
