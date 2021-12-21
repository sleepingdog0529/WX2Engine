#include "mesh.h"

namespace wx2
{
	void Mesh::Initialize(
		Device* devices,
		std::span<ModelVertex> vertices,
		std::span<DWORD> indices,
		std::unordered_multimap<TextureType, std::shared_ptr<Texture>>& textures)
	{
		Drawable::Initialize(devices);

		vertices_ = {
			std::make_move_iterator(vertices.begin()),
			std::make_move_iterator(vertices.end()) };

		indices_ = {
			std::make_move_iterator(indices.begin()),
			std::make_move_iterator(indices.end()) };

		textures_ = {
			std::make_move_iterator(textures.begin()),
			std::make_move_iterator(textures.end()) };

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
			return;

		itr->second.BindPS(slot);
	}
}