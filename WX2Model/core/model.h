/*********************************************************************
 * @file   model.h
 * @author Tomomi Murakami
 * @date   2021/11/29 18:11
 * @brief  ƒ‚ƒfƒ‹
 ********************************************************************/
#pragma once
#include <assimp/scene.h>
#include "mesh.h"

namespace wx2
{
	class Model final
	{
	public:
		Model(Device* devices, const std::filesystem::path& filePath);
		~Model() = default;

		void Draw() const noexcept;

		WX2_COPYABLE(Model);
		WX2_MOVEABLE(Model);

	private:
		void Load();
		void ProcessNode(const aiNode* aiNode, const aiScene* aiScene) noexcept;
		std::shared_ptr<Mesh> ProcessMesh(const aiMesh* aiMesh, const aiScene* aiScene) noexcept;
		std::unordered_map<TextureType, std::shared_ptr<Texture>> LoadMaterialTextures(
			const aiMaterial* aiMat, const aiScene* aiScene) const noexcept;

		Device* devices_;
		std::filesystem::path filePath_;
		std::vector<std::shared_ptr<Mesh>> meshes_{};
	};
}
