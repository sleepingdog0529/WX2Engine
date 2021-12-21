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

	std::shared_ptr<Model> ModelLoader::Load(const std::string& key, const std::filesystem::path& filePath)
	{
		WX2_ASSERT_MSG(std::filesystem::exists(filePath), "モデルファイルが存在しません。");;

		const auto directory = filePath.parent_path();

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
		return ProcessNode(meshes, scene->mRootNode, scene, directory);
	}

	std::shared_ptr<Mesh> ModelLoader::ProcessMesh(
		const aiMesh* aiMesh,
		const aiScene* aiScene,
		const std::filesystem::path& directory) const noexcept
	{
		std::vector<ModelVertex> vertices;
		std::vector<DWORD> indices;

		for (int i = 0; i < aiMesh->mNumVertices; i++)
		{
			ModelVertex vertex{};

			vertex.position.x = aiMesh->mVertices[i].x;
			vertex.position.y = aiMesh->mVertices[i].y;
			vertex.position.z = aiMesh->mVertices[i].z;

			if (aiMesh->HasTextureCoords(0))
			{
				vertex.texcoord.x = aiMesh->mTextureCoords[0][i].x;
				vertex.texcoord.y = aiMesh->mTextureCoords[0][i].y;
			}

			if (aiMesh->HasNormals())
			{
				vertex.normal.x = aiMesh->mNormals[i].x;
				vertex.normal.y = aiMesh->mNormals[i].y;
				vertex.normal.z = aiMesh->mNormals[i].z;

				if (aiMesh->HasTangentsAndBitangents())
				{
					vertex.tangent.x = aiMesh->mTangents[i].x;
					vertex.tangent.y = aiMesh->mTangents[i].y;
					vertex.tangent.z = aiMesh->mTangents[i].z;

					vertex.binormal.x = aiMesh->mBitangents[i].x;
					vertex.binormal.y = aiMesh->mBitangents[i].y;
					vertex.binormal.z = aiMesh->mBitangents[i].z;
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

		TexturesMap textures;
		LoadMaterialTextures(aiMat, aiScene, aiTextureType_DIFFUSE, directory, textures);

		auto mesh = std::make_shared<Mesh>();
		mesh->Initialize(devices_, vertices, indices, textures);
		return mesh;
	}

	std::shared_ptr<Model> ModelLoader::ProcessNode(
		std::vector<std::shared_ptr<Mesh>>& meshes,
		const aiNode* aiNode,
		const aiScene* aiScene,
		const std::filesystem::path& directory) noexcept
	{
		for (UINT i = 0; i < aiNode->mNumMeshes; i++)
		{
			const aiMesh* mesh = aiScene->mMeshes[aiNode->mMeshes[i]];

			meshes.push_back(ProcessMesh(mesh, aiScene, directory));
		}

		for (UINT i = 0; i < aiNode->mNumChildren; i++)
		{
			ProcessNode(meshes, aiNode->mChildren[i], aiScene, directory);
		}

		auto model = std::make_shared<Model>();
		model->Initialize(devices_, meshes);
		return model;
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

	void ModelLoader::LoadMaterialTextures(
		const aiMaterial* aiMat,
		const aiScene* aiScene,
		const aiTextureType texType,
		const std::filesystem::path& directory,
		TexturesMap& out) const
	{
		const auto texCount = aiMat->GetTextureCount(texType);

		if (texCount > 0)
		{
			for (int j = 0; j < texCount; j++)
			{
				aiString path;
				aiMat->GetTexture(texType, j, &path);

				std::shared_ptr<Texture> texture;

				switch (DetermineTextureStorageType(aiScene, aiMat, j, texType))
				{
				case TextureStorageType::EmbeddedIndexCompressed:
				{
					const int index = boost::lexical_cast<int>(&path.C_Str()[1]);
					texture = std::make_shared<Texture>(
						devices_,
						reinterpret_cast<uint8_t*>(aiScene->mTextures[index]->pcData),
						aiScene->mTextures[index]->mWidth);
					break;
				}
				case TextureStorageType::EmbeddedCompressed:
				{
					const aiTexture* aiTexture =
						aiScene->GetEmbeddedTexture(path.C_Str());
					texture = std::make_shared<Texture>(
						devices_,
						reinterpret_cast<uint8_t*>(aiTexture->pcData),
						aiTexture->mWidth);
					break;
				}
				case TextureStorageType::Disk:
				{
					const auto fileName = directory / path.C_Str();
					texture = std::make_shared<Texture>(devices_, fileName);
					break;
				}
				default:
					WX2_LOG_WARN("非対応のテクスチャ保存形式です。");
					break;
				}

				if (!texture)
					continue;

				out.emplace(
					static_cast<TextureType>(texType),
					std::move(texture));
			}
		}
		else
		{
			std::shared_ptr<Texture> texture;

			switch (texType)
			{
			case aiTextureType_DIFFUSE:
			{
				aiColor3D aiColor;
				aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
				
				texture = std::make_shared<Texture>(
					devices_,
					Color(aiColor.r, aiColor.g, aiColor.b, 1.0f));

				out.emplace(
					static_cast<TextureType>(texType),
					std::move(texture));
				break;
			}
			case aiTextureType_NORMALS:
			{
				texture = std::make_shared<Texture>(
					devices_, 
					Color(0.5f, 0.5f, 1.0f, 1.0f));
				break;
			}
			default:
				break;
			}
		}
	}
}