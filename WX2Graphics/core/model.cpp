#include "model.h"

namespace wx2
{
	void Model::Initialize(
		Device* devices,
		std::span<std::shared_ptr<Mesh>> meshes) noexcept
	{
		Drawable::Initialize(devices);

		meshes_ = { meshes.begin(), meshes.end() };
	}

	void Model::Draw() const noexcept
	{
		for (const auto& m : meshes_)
		{
			m->Draw();
		}
	}
}