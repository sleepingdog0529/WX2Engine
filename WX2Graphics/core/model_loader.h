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

	class ModelLoader
	{
	private:
		using ModelPtr = std::shared_ptr<Model>;

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
			const DirectX::XMMATRIX transformMatrix) const noexcept;

		Model ProcessNode(
			std::vector<Mesh>& meshes,
			const aiNode* aiNode,
			const aiScene* aiScene, 
			const DirectX::XMMATRIX parentTransformMatrix) noexcept;

		TextureStorageType DetermineTextureStorageType(const aiScene* aiScene, const aiMaterial* aiMaterial, UINT index, aiTextureType aiTextureType) const;

		std::map<std::string, Texture> LoadMaterialTextures(
			const aiMaterial* aiMat,
			aiTextureType aiType,
			std::string typeName,
			const aiScene* aiScene);

		Device* devices_{};
		ConstantBuffer<WVPMatrix>* constantBufferWVP_;
	};
}
