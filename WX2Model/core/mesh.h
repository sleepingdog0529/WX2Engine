/*********************************************************************
 * @file   mesh.h
 * @author Tomomi Murakami
 * @date   2021/11/29 17:16
 * @brief  ÉÅÉbÉVÉÖ
 ********************************************************************/
#pragma once
#include <vector>
#include <span>
#include <map>
#include <WX2Mathematics.h>

#include <WX2Graphics.h>
#include "texture.h"
#include "texture_type.h"

namespace wx2
{
	class Mesh final
	{
	public:
		Mesh(
			Device* devices,
			std::span<ModelVertex> vertices,
			std::span<DWORD> indices,
			std::unordered_map<TextureType, std::shared_ptr<Texture>>& textures);
		~Mesh() = default;

		void Draw() const noexcept;

	private:
		void PSBindTexture(const UINT slot, const TextureType& texType) const noexcept;

		Device* devices_{};
		VertexBuffer<ModelVertex> vertexBuffer_;
		IndexBuffer indexBuffer_;
		std::vector<ModelVertex> vertices_{};
		std::vector<DWORD> indices_{};
		std::unordered_map<TextureType, std::shared_ptr<Texture>> textures_{};
	};
}
