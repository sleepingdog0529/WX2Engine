/*********************************************************************
 * @file   model.h
 * @author Tomomi Murakami
 * @date   2021/11/29 18:11
 * @brief  ƒ‚ƒfƒ‹
 ********************************************************************/
#pragma once
#include "mesh.h"
#include <WX2Mathematics.h>

namespace wx2
{
	class Model final
	{
	public:
		Model(Device* devices, std::span<std::shared_ptr<Mesh>> meshes) noexcept;
		~Model() = default;

		void Draw() const noexcept;

	private:
		Device* devices_;
		std::vector<std::shared_ptr<Mesh>> meshes_{};
	};
}
