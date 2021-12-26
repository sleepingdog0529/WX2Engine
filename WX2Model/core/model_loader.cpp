#include "model_loader.h"
#include <boost/lexical_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace wx2
{
	void ModelLoader::Initialize(Device* devices)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");

		devices_ = devices;
	}

	std::shared_ptr<Model> ModelLoader::Load(const std::filesystem::path& filePath)
	{
		WX2_ASSERT_MSG(std::filesystem::exists(filePath), "モデルファイルが存在しません。");;

		if (auto model = modelPool_.Get(filePath.string()))
		{
			return model;
		}

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(
			filePath.string(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace);
		WX2_RUNTIME_ERROR_IF_FAILED(scene, "モデルファイルの読み込みに失敗しました。パス: {}", filePath.string());

		std::vector<std::shared_ptr<Mesh>> meshes;
		return ProcessNode(meshes, scene->mRootNode, scene, filePath);
	}

	std::shared_ptr<Mesh> ModelLoader::ProcessMesh(
		const aiMesh* aiMesh,
		const aiScene* aiScene,
		const std::filesystem::path& filePath) const noexcept
	{
		std::vector<ModelVertex> vertices;
		std::vector<DWORD> indices;

		for (int i = 0; i < aiMesh->mNumVertices; i++)
		{
			ModelVertex vertex{};

			vertex.position = {
				aiMesh->mVertices[i].x,
				aiMesh->mVertices[i].y,
				aiMesh->mVertices[i].z };

			if (aiMesh->HasTextureCoords(0))
			{
				vertex.texcoord = {
					aiMesh->mTextureCoords[0][i].x,
					aiMesh->mTextureCoords[0][i].y };
			}

			if (aiMesh->HasNormals())
			{
				vertex.normal = {
					aiMesh->mNormals[i].x,
					aiMesh->mNormals[i].y,
					aiMesh->mNormals[i].z };

				if (aiMesh->HasTangentsAndBitangents())
				{
					vertex.tangent = {
						aiMesh->mTangents[i].x,
						aiMesh->mTangents[i].y,
						aiMesh->mTangents[i].z };

					vertex.binormal = {
						aiMesh->mBitangents[i].x,
						aiMesh->mBitangents[i].y,
						aiMesh->mBitangents[i].z };
				}
			}

			vertices.push_back(vertex);
		}

		for (int i = 0; i < aiMesh->mNumFaces; i++)
		{
			const aiFace face = aiMesh->mFaces[i];

			for (int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		const aiMaterial* aiMat = aiScene->mMaterials[aiMesh->mMaterialIndex];
		auto textures = LoadMaterialTextures(aiMat, aiScene, filePath);
		
		return std::make_shared<Mesh>(devices_, vertices, indices, textures);
	}

	std::shared_ptr<Model> ModelLoader::ProcessNode(
		std::vector<std::shared_ptr<Mesh>>& meshes,
		const aiNode* aiNode,
		const aiScene* aiScene,
		const std::filesystem::path& filePath) noexcept
	{
		for (UINT i = 0; i < aiNode->mNumMeshes; i++)
		{
			const aiMesh* mesh = aiScene->mMeshes[aiNode->mMeshes[i]];

			meshes.push_back(ProcessMesh(mesh, aiScene, filePath));
		}

		for (UINT i = 0; i < aiNode->mNumChildren; i++)
		{
			ProcessNode(meshes, aiNode->mChildren[i], aiScene, filePath);
		}

		if (aiNode == aiScene->mRootNode)
			return modelPool_.Emplace(filePath.string(), devices_, meshes);
	}

	TextureStorageType ModelLoader::DetermineTextureStorageType(
		const aiScene* aiScene,
		const aiMaterial* aiMaterial,
		const UINT index,
		const aiTextureType aiTextureType) const
	{
		if (aiMaterial->GetTextureCount(aiTextureType) == 0)
		{
			return TextureStorageType::None;
		}

		aiString path;
		aiMaterial->GetTexture(aiTextureType, index, &path);
		std::string texturePath = path.C_Str();

		if (texturePath[0] == '*')
		{
			if (aiScene->mTextures[0]->mHeight == 0)
			{
				return TextureStorageType::EmbeddedIndexCompressed;
			}
			else
			{
				WX2_LOG_ERROR("インデックス付き非圧縮テクスチャは読み込むことが出来ません。");
				return TextureStorageType::EmbeddedIndexNonCompressed;
			}
		}
		if (const auto tex = aiScene->GetEmbeddedTexture(texturePath.c_str()))
		{
			if (tex->mHeight == 0)
			{
				return TextureStorageType::EmbeddedCompressed;
			}
			else
			{
				WX2_LOG_ERROR("埋め込み非圧縮テクスチャは読み込むことが出来ません。");
				return TextureStorageType::EmbeddedNonCompressed;
			}
		}
		if (texturePath.find('.') != std::string::npos)
		{
			return TextureStorageType::Disk;
		}

		return TextureStorageType::None;
	}

	std::unordered_map<TextureType, std::shared_ptr<Texture>> ModelLoader::LoadMaterialTextures(
		const aiMaterial* aiMat,
		const aiScene* aiScene,
		const std::filesystem::path& filePath) const
	{
		std::unordered_map<TextureType, std::shared_ptr<Texture>> textures;

		for (int i = 0; i < aiTextureType::aiTextureType_UNKNOWN; i++)
		{
			const auto aiTexType = static_cast<aiTextureType>(i);
			const auto texCount = aiMat->GetTextureCount(aiTexType);

			const auto texTypeItr = TEXTURE_TYPE_TRANSLATOR.find(aiTexType);
			if (texTypeItr == TEXTURE_TYPE_TRANSLATOR.end())
			{
				continue;
			}

			const auto texType = texTypeItr->second;

			std::shared_ptr<Texture> texture;

			if (texCount > 0)
			{
				for (int j = 0; j < texCount; j++)
				{
					aiString path;
					aiMat->GetTexture(aiTexType, j, &path);

					switch (DetermineTextureStorageType(aiScene, aiMat, j, aiTexType))
					{
					case TextureStorageType::EmbeddedIndexCompressed:
					{
						const int index = boost::lexical_cast<int>(&path.C_Str()[1]);

						const auto data = std::span(
							reinterpret_cast<uint8_t*>(aiScene->mTextures[index]->pcData), 
							aiScene->mTextures[index]->mWidth);

						texture = std::make_shared<Texture>(devices_, data);

						break;
					}
					case TextureStorageType::EmbeddedCompressed:
					{
						const aiTexture* aiTexture =
							aiScene->GetEmbeddedTexture(path.C_Str());

						const auto data = std::span(
							reinterpret_cast<uint8_t*>(aiTexture->pcData),
							aiTexture->mWidth);

						texture = std::make_shared<Texture>(devices_, data);
						
						break;
					}
					case TextureStorageType::Disk:
					{
						std::filesystem::path fileName(filePath.parent_path());
						fileName /= path.C_Str();

						texture = std::make_shared<Texture>(devices_, fileName.string());

						break;
					}
					default:
						WX2_LOG_WARN("非対応のテクスチャ保存形式です。");
						break;
					}

					if(texture)
						textures.emplace(texType, std::move(texture));
					texture = nullptr;
				}
			}
			else
			{
				switch (texType)
				{
				case TextureType::Diffuse:
				{
					aiColor3D aiColor;
					aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);

					const auto color = Color(aiColor.r, aiColor.g, aiColor.b, 1.0f);

					texture = std::make_shared<Texture>(devices_, color);

					break;
				}
				case TextureType::Normals:
				{
					constexpr auto color = Color(0.5f, 0.5f, 1.0f, 1.0f);

					texture = std::make_shared<Texture>(devices_, color);

					break;
				}
				default:
					break;
				}

				if(texture)
					textures.emplace(texType, std::move(texture));
				texture = nullptr;
			}
		}
		return textures;
	}
}