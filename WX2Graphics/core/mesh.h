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
#include "drawable.h"
#include "vertex_type.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"
#include "material_type.h"

namespace wx2
{
	class Mesh final : public Drawable
	{
	public:
		Mesh() = default;
		~Mesh() override = default;

		WX2_COPYABLE(Mesh);
		WX2_MOVEABLE(Mesh);

		void Initialize(
			Device* devices,
			std::span<ModelVertex> vertices,
			std::span<DWORD> indices,
			std::unordered_multimap<TextureType, std::shared_ptr<Texture>>& textures);

		void Draw() const noexcept override;

	private:
		void PSBindTexture(const UINT slot, const TextureType& texType) const noexcept;
		
		VertexBuffer<ModelVertex> vertexBuffer_{};
		IndexBuffer indexBuffer_{};
		std::vector<ModelVertex> vertices_{};
		std::vector<DWORD> indices_{};
		std::unordered_multimap<TextureType, Texture> textures_{};
	};
}
