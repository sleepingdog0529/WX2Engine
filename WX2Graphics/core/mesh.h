/*********************************************************************
 * @file   mesh.h
 * @author Tomomi Murakami
 * @date   2021/11/29 17:16
 * @brief  ÉÅÉbÉVÉÖ
 ********************************************************************/
#pragma once
#include <vector>
#include <span>
#include <DirectXMath.h>
#include <map>
#include <WX2Mathematics.h>

#include "device.h"
#include "vertex_type.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"
#include "texture_type.h"

namespace wx2::graphics
{
	class Mesh final
	{
	public:
		Mesh() = default;
		~Mesh() = default;

		void Initialize(
			Device* devices,
			std::span<ModelVertex> vertices,
			std::span<DWORD> indices,
			const std::map<TextureType, Texture>& textures,
			const Matrix& transformMatrix);

		void Draw() const noexcept;

		[[nodiscard]] const DirectX::XMMATRIX& GetTransformMatrix() const noexcept { return transformMatrix_; }

	private:
		void PSBindTexture(const UINT slot, const TextureType& texType) const noexcept;

		Device* devices_{};
		VertexBuffer<ModelVertex> vertexBuffer_;
		IndexBuffer indexBuffer_;
		std::vector<ModelVertex> vertices_{};
		std::vector<DWORD> indices_{};
		std::map<TextureType, Texture> textures_{};
		DirectX::XMMATRIX transformMatrix_{};
	};
}
