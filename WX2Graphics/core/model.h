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
#include <WX2Mathematics.h>

namespace wx2
{
	class Model final : public Drawable
	{
	public:
		Model() = default;
		~Model() override = default;

		WX2_COPYABLE(Model);
		WX2_MOVEABLE(Model);

		void Initialize(
			Device* devices,
			std::span<std::shared_ptr<Mesh>> meshes) noexcept;

		void Draw() const noexcept override;

	private:
		std::vector<std::shared_ptr<Mesh>> meshes_{};
	};
}
