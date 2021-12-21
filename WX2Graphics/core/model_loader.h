/*********************************************************************
 * @file   model_loader.h
 * @author Tomomi Murakami
 * @date   2021/11/29 16:07
 * @brief  ÉÇÉfÉãì«çû
 ********************************************************************/
#pragma once
#include <filesystem>
#include <assimp/scene.h>
#include "model.h"
#include "model_loader.h"
#include "material_type.h"
#include "model_storage.h"

namespace wx2
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
		using TexturesMap = std::unordered_multimap<TextureType, std::shared_ptr<Texture>>;

	public:
		ModelLoader() = default;
		~ModelLoader() = default;

		WX2_COPYABLE(ModelLoader);
		WX2_MOVEABLE(ModelLoader);

		void Initialize(Device* devices);

		std::shared_ptr<Model> Load(
			const std::string& key,
			const std::filesystem::path& filePath);

	private:
		std::shared_ptr<Mesh> ProcessMesh(
			const aiMesh* aiMesh,
			const aiScene* aiScene,
			const std::filesystem::path& directory) const noexcept;

		std::shared_ptr<Model> ProcessNode(
			std::vector<std::shared_ptr<Mesh>>& meshes,
			const aiNode* aiNode,
			const aiScene* aiScene,
			const std::filesystem::path& directory) noexcept;

		TextureStorageType DetermineTextureStorageType(
			const aiScene* aiScene,
			const aiMaterial* aiMaterial,
			UINT index,
			aiTextureType aiTextureType) const;

		void LoadMaterialTextures(
			const aiMaterial* aiMat,
			const aiScene* aiScene,
			const aiTextureType texType,
			const std::filesystem::path& directory,
			TexturesMap& out) const;

		struct MatKeyTranslate
		{
			MatKeyType keyType;
			struct
			{
				std::string key;
				int type;
				int index;
			} matKey;
		};

		static inline const MatKeyTranslate MAT_KEY_TRANSLATOR[] = {
			{ MatKeyType::ColorSpecular		, { AI_MATKEY_COLOR_SPECULAR } },
			{ MatKeyType::ColorDiffuse		, { AI_MATKEY_COLOR_DIFFUSE  } },
			{ MatKeyType::ColorAmbient		, { AI_MATKEY_COLOR_AMBIENT  } },
			{ MatKeyType::Shininess			, { AI_MATKEY_SHININESS      } },
			{ MatKeyType::ShininessStrength	, { AI_MATKEY_SHININESS      } } };

		Device* devices_{};
		ModelStorage storage_{};
	};
}
