#include "model.h"
#include <boost/lexical_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "assimp_helper.h"

namespace wx2
{
	Model::Model(Device* devices, const std::filesystem::path& filePath)
		: devices_(devices)
		, filePath_(filePath)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");

		if (!std::filesystem::exists(filePath_))
		{
			WX2_LOG_ERROR("モデルファイルが存在しません。パス: {}", filePath_.string());
			return;
		}

		Load();
	}

	void Model::Draw() const noexcept
	{
		for (const auto& m : meshes_)
		{
			m->Draw();
		}
	}

	void Model::Load()
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(
			filePath_.string(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace);
		WX2_RUNTIME_ERROR_IF_FAILED(scene, "モデルファイルの読み込みに失敗しました。パス: {}", filePath_.string());

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(
		const aiNode* aiNode,
		const aiScene* aiScene) noexcept
	{
		for (UINT i = 0; i < aiNode->mNumMeshes; i++)
		{
			const aiMesh* mesh = aiScene->mMeshes[aiNode->mMeshes[i]];

			meshes_.push_back(ProcessMesh(mesh, aiScene));
		}

		for (UINT i = 0; i < aiNode->mNumChildren; i++)
		{
			ProcessNode(aiNode->mChildren[i], aiScene);
		}
	}

	std::shared_ptr<Mesh> Model::ProcessMesh(
		const aiMesh* aiMesh,
		const aiScene* aiScene) noexcept
	{
		std::vector<ModelVertex> vertices(aiMesh->mNumVertices);
		std::vector<DWORD> indices(aiMesh->mNumFaces * 3);

		for (std::size_t i = 0; i < aiMesh->mNumVertices; ++i)
		{
			vertices[i].position = {
				aiMesh->mVertices[i].x,
				aiMesh->mVertices[i].y,
				aiMesh->mVertices[i].z };

			if (aiMesh->HasTextureCoords(0))
			{
				vertices[i].texcoord = {
					aiMesh->mTextureCoords[0][i].x,
					aiMesh->mTextureCoords[0][i].y };
			}

			if (aiMesh->HasNormals())
			{
				vertices[i].normal = {
					aiMesh->mNormals[i].x,
					aiMesh->mNormals[i].y,
					aiMesh->mNormals[i].z };

				if (aiMesh->HasTangentsAndBitangents())
				{
					vertices[i].tangent = {
						aiMesh->mTangents[i].x,
						aiMesh->mTangents[i].y,
						aiMesh->mTangents[i].z };

					vertices[i].binormal = {
						aiMesh->mBitangents[i].x,
						aiMesh->mBitangents[i].y,
						aiMesh->mBitangents[i].z };
				}
			}
		}

		for (std::size_t i = 0; i < aiMesh->mNumFaces; ++i)
		{
			const aiFace face = aiMesh->mFaces[i];

			for (std::size_t j = 0; j < 3; ++j)
			{
				indices[i * 3 + j] = face.mIndices[j];
			}
		}

		const aiMaterial* aiMat = aiScene->mMaterials[aiMesh->mMaterialIndex];
		auto textures = LoadMaterialTextures(aiMat, aiScene);

		return std::make_shared<Mesh>(devices_, vertices, indices, textures);
	}

	std::unordered_map<TextureType, std::shared_ptr<Texture>> Model::LoadMaterialTextures(
		const aiMaterial* aiMat,
		const aiScene* aiScene) const noexcept
	{
		std::unordered_map<TextureType, std::shared_ptr<Texture>> textures;

		for (int i = 0; i < aiTextureType_UNKNOWN; i++)
		{
			const auto aiTexType = static_cast<aiTextureType>(i);
			const auto texCount = aiMat->GetTextureCount(aiTexType);

			const auto texType = ToTextureType(aiTexType);
			if (texType == TextureType::Unknown)
				continue;

			std::shared_ptr<Texture> texture;

			if (texCount > 0)
			{
				for (UINT j = 0; j < texCount; j++)
				{
					aiString path;
					aiMat->GetTexture(aiTexType, j, &path);

					switch (ParseTextureStorageType(aiScene, aiMat, j, aiTexType))
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
						std::filesystem::path fileName(filePath_.parent_path());
						fileName /= path.C_Str();

						texture = std::make_shared<Texture>(devices_, fileName.string());

						break;
					}
					default:
						WX2_LOG_WARN("非対応のテクスチャ保存形式です。");
						continue;
					}

					textures.insert_or_assign(texType, std::move(texture));
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
					continue;
				}

				textures.emplace(texType, std::move(texture));
			}
		}
		return textures;
	}
}