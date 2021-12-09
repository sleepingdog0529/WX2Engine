#include "model.h"

namespace wx2::graphics
{
	void Model::Initialize(
		Device* devices,
		ConstantBuffer<WVPMatrix>* constantBufferWVP,
		std::span<Mesh> meshes)
	{
		WX2_ASSERT_MSG(devices, "�f�o�C�X��nullptr�ł����B");
		WX2_ASSERT_MSG(constantBufferWVP, "�萔�o�b�t�@��nullptr�ł����B");

		devices_ = devices;
		constantBufferWVP_ = constantBufferWVP;

		std::ranges::copy(meshes, std::back_inserter(meshes_));
	}

	void Model::Draw(const DirectX::XMMATRIX& worldMatrix) const noexcept
	{
		for (const auto& m : meshes_)
		{
			constantBufferWVP_->data.world = Matrix(worldMatrix * m.GetTransformMatrix());
			constantBufferWVP_->ApplyChange();

			m.Draw();
		}
	}
}