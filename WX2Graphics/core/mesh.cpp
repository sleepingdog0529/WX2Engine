#include "mesh.h"

namespace wx2
{
	void Mesh::Initialize(
		Device* devices,
		std::span<ModelVertex> vertices,
		std::span<DWORD> indices,
		const std::map<TextureType, Texture>& textures)
	{
		WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");

		devices_ = devices;

		vertices_.resize(vertices.size());
		std::ranges::move(vertices, vertices_.begin());

		indices_.resize(indices.size());
		std::ranges::move(indices, indices_.begin());

		textures_ = std::move(textures);

		vertexBuffer_.Initialize(devices_, vertices_);
		indexBuffer_.Initialize(devices_, indices_);
	}

	void Mesh::Draw() const noexcept
	{
		vertexBuffer_.Bind();
		indexBuffer_.Bind();

		PSBindTexture(0, TextureType::Diffuse);

		auto* devCon = devices_->GetDeviceContext();

		devCon->DrawIndexed(static_cast<UINT>(indices_.size()), 0, 0);
	}

	void Mesh::PSBindTexture(const UINT slot, const TextureType& texType) const noexcept
	{
		const auto itr = textures_.find(texType);
		if (itr == textures_.end())
		{
			return;
		}

		itr->second.BindPS(slot);
	}
}