/*********************************************************************
 * @file   model.h
 * @author Tomomi Murakami
 * @date   2021/11/29 18:11
 * @brief  ƒ‚ƒfƒ‹
 ********************************************************************/
#pragma once
#include "mesh.h"
#include "constant_buffer.h"
#include "constant_buffer_type.h"

namespace wx2::graphics
{
	class Model final
	{
	public:
		Model() = default;
		~Model() = default;

		void Initialize(
			Device* devices,
			ConstantBuffer<WVPMatrix>* constantBufferWVP,
			std::span<Mesh> meshes);

		void Draw(const Matrix& worldMatrix) const noexcept;

	private:
		Device* devices_;
		ConstantBuffer<WVPMatrix>* constantBufferWVP_;
		std::vector<Mesh> meshes_{};
	};
}
