#include "model_loader.h"
#include <boost/lexical_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
namespace wx2::graphics
{
	void ModelLoader::Initialize(
		Device* devices,
		ConstantBuffer<WVPMatrix>* constantBufferWVP)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
		WX2_ASSERT_MSG(constantBufferWVP, "定数バッファがnullptrでした。");

		devices_ = devices;
		constantBufferWVP_ = constantBufferWVP;
	}

	Model ModelLoader::Load(const std::filesystem::path& filePath)
	{
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
		return ProcessNode(meshes, scene->mRootNode, scene, DirectX::XMMatrixIdentity());
	}

	Mesh ModelLoader::ProcessMesh(
		const aiMesh* aiMesh,
		const aiScene* aiScene,
		const DirectX::XMMATRIX transformMatrix) const noexcept
	{
		std::vector<ModelVertex> vertices;
		std::vector<DWORD> indices;

		for (int i = 0; i < aiMesh->mNumVertices; i++)
		{
			ModelVertex vertex{};

			vertex.position.x = aiMesh->mVertices[i].x;
			vertex.position.y = aiMesh->mVertices[i].y;
			vertex.position.z = aiMesh->mVertices[i].z;

			//if (aiMesh->mTextureCoords[0]) {
			//	vertex.texcoord.x = (float)aiMesh->mTextureCoords[0][i].x;
			//	vertex.texcoord.y = (float)aiMesh->mTextureCoords[0][i].y;
			//}

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

		Mesh mesh;
		mesh.Initialize(devices_, vertices, indices, transformMatrix);
		return mesh;
	}

	Model ModelLoader::ProcessNode(
		std::vector<Mesh>& meshes, 
		const aiNode* aiNode,
		const aiScene* aiScene, 
		const DirectX::XMMATRIX parentTransformMatrix) noexcept
	{
		const DirectX::XMMATRIX nodeTransformMatrix =
			XMMatrixTranspose(DirectX::XMMATRIX(&aiNode->mTransformation.a1)) * parentTransformMatrix;

		for (UINT i = 0; i < aiNode->mNumMeshes; i++) 
		{
			const aiMesh* mesh = aiScene->mMeshes[aiNode->mMeshes[i]];

			meshes.push_back(ProcessMesh(mesh, aiScene, nodeTransformMatrix));
		}

		for (UINT i = 0; i < aiNode->mNumChildren; i++) 
		{
			ProcessNode(meshes, aiNode->mChildren[i], aiScene, nodeTransformMatrix);
		}

		Model model;
		model.Initialize(devices_, constantBufferWVP_, meshes);
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

	std::map<std::string, Texture> ModelLoader::LoadMaterialTextures(
		const aiMaterial* aiMat,
		const aiTextureType aiType,
		std::string typeName, 
		const aiScene* aiScene)
	{
		std::map<std::string, Texture> textures;

		const auto texCount = aiMat->GetTextureCount(aiType);
		for (UINT i = 0; i < texCount; i++)
		{
			aiString path;
			aiMat->GetTexture(aiType, i, &path);

			// TODO: 既にロードしてあればそれを使うようにする

			const auto storeType = DetermineTextureStorageType(aiScene, aiMat, i, aiType);
			switch (storeType)
			{
			case TextureStorageType::EmbeddedIndexCompressed:
				{
					const int index = boost::lexical_cast<int>(&path.C_Str()[1]);
					Texture embeddedIndexedTexture;
					embeddedIndexedTexture.Initialize(
						devices_, 
						reinterpret_cast<uint8_t*>(aiScene->mTextures[index]->pcData),
						aiScene->mTextures[index]->mWidth);
					break;
				}
			}
		}
	}
}
