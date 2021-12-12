#include "model.h"

namespace wx2
{
	void Model::Initialize(
		Device* devices,
		ConstantBuffer<WVPMatrix>* constantBufferWVP,
		std::span<Mesh> meshes)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
		WX2_ASSERT_MSG(constantBufferWVP, "定数バッファがnullptrでした。");

		devices_ = devices;
		constantBufferWVP_ = constantBufferWVP;

		std::ranges::copy(meshes, std::back_inserter(meshes_));
	}

	void Model::Draw(const Matrix& worldMatrix) const noexcept
	{
		for (const auto& m : meshes_)
		{
			constantBufferWVP_->data.world = worldMatrix * m.GetTransformMatrix();
			constantBufferWVP_->ApplyChange();

			m.Draw();
		}
	}
}