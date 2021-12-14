#include "model.h"

namespace wx2
{
	void Model::Initialize(
		Device* devices,
		std::span<Mesh> meshes)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");

		devices_ = devices;

		std::ranges::copy(meshes, std::back_inserter(meshes_));
	}

	void Model::Draw() const noexcept
	{
		for (const auto& m : meshes_)
		{
			m.Draw();
		}
	}
}