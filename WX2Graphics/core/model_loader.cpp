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

	Model ModelLoader::Load(const std::filesystem::path& filePath)
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

		std::vector<Mesh> meshes;
		return ProcessNode(meshes, scene->mRootNode, scene, directory);
	}

	Mesh ModelLoader::ProcessMesh(
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
		const auto textures = LoadMaterialTextures(aiMat, aiScene, directory);

		Mesh mesh;
		mesh.Initialize(devices_, vertices, indices, textures);
		return mesh;
	}

	Model ModelLoader::ProcessNode(
		std::vector<Mesh>& meshes,
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

		Model model;
		model.Initialize(devices_, meshes);
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

	std::map<TextureType, Texture> ModelLoader::LoadMaterialTextures(
		const aiMaterial* aiMat,
		const aiScene* aiScene,
		const std::filesystem::path& directory) const
	{
		std::map<TextureType, Texture> textures;

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

			if (texCount > 0)
			{
				for (int j = 0; j < texCount; j++)
				{
					aiString path;
					aiMat->GetTexture(aiTexType, j, &path);

					// TODO: 既にロードしてあればそれを使うようにする

					switch (DetermineTextureStorageType(aiScene, aiMat, j, aiTexType))
					{
					case TextureStorageType::EmbeddedIndexCompressed:
					{
						const int index = boost::lexical_cast<int>(&path.C_Str()[1]);

						Texture embeddedIndexedTexture;
						embeddedIndexedTexture.Initialize(
							devices_,
							reinterpret_cast<uint8_t*>(aiScene->mTextures[index]->pcData),
							aiScene->mTextures[index]->mWidth);

						textures.emplace(texType, std::move(embeddedIndexedTexture));
						break;
					}
					case TextureStorageType::EmbeddedCompressed:
					{
						const aiTexture* texture =
							aiScene->GetEmbeddedTexture(path.C_Str());

						Texture embeddedTexture;
						embeddedTexture.Initialize(
							devices_,
							reinterpret_cast<uint8_t*>(texture->pcData),
							texture->mWidth);

						textures.emplace(texType, std::move(embeddedTexture));
						break;
					}
					case TextureStorageType::Disk:
					{
						const auto fileName = directory / path.C_Str();

						Texture diskTexture;
						diskTexture.Initialize(devices_, fileName);

						textures.emplace(texType, std::move(diskTexture));
						break;
					}
					default:
						WX2_LOG_WARN("非対応のテクスチャ保存形式です。");
						break;
					}
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

					Texture diffuseTexture;
					diffuseTexture.Initialize(
						devices_,
						{ aiColor.r, aiColor.g, aiColor.b, 1.0f });

					textures.emplace(texType, std::move(diffuseTexture));
					break;
				}
				case TextureType::Normals:
				{
					Texture normalTexture;
					normalTexture.Initialize(
						devices_,
						{ 0.5f, 0.5f, 1.0f, 1.0f });

					textures.emplace(texType, std::move(normalTexture));
					break;
				}
				default:
					break;
				}
			}
		}
		return textures;
	}
}