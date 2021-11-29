#include "mesh.h"

namespace wx2::graphics
{
	void Mesh::Initialize(
		Device* devices, 
		std::span<ModelVertex> vertices,
		std::span<DWORD> indices,
		const std::map<std::string, Texture>& textures,
		const DirectX::XMMATRIX& transformMatrix)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");

		devices_ = devices;
		transformMatrix_ = transformMatrix;

		vertices_.resize(vertices.size());
		std::ranges::copy(vertices, vertices_.begin());

		indices_.resize(indices.size());
		std::ranges::copy(indices, indices_.begin());

		indices_.resize(textures.size());
		std::copy(textures.begin(), textures.end(), textures_.begin());

		vertexBuffer_.Initialize(devices_, vertices_);
		indexBuffer_.Initialize(devices_, indices_);
	}

	void Mesh::Draw() const noexcept
	{
		vertexBuffer_.Bind();
		indexBuffer_.Bind();

		auto* devCon =  devices_->GetDeviceContext();

		devCon->DrawIndexed(static_cast<UINT>(indices_.size()), 0, 0);
	}
}
