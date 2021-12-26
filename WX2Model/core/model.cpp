#include "model.h"

namespace wx2
{
	Model::Model(Device* devices, std::span<std::shared_ptr<Mesh>> meshes) noexcept
		: devices_(devices)
	{
		WX2_ASSERT_MSG(devices, "�f�o�C�X��nullptr�ł����B");

		meshes_.resize(meshes.size());
		meshes_ = {
			std::make_move_iterator(meshes.begin()),
			std::make_move_iterator(meshes.end()) };
	}

	void Model::Draw() const noexcept
	{
		for (const auto& m : meshes_)
		{
			m->Draw();
		}
	}
}