#include "mesh.h"

namespace wx2 {

	Mesh::Mesh(
		Graphics* graphics, 
		std::span<Vertex3D> vertices,
		std::span<UINT> indices,
		std::vector<Mesh>& children) :
		graphics_(graphics),
		vertex_buffer_(graphics, vertices),
		index_buffer_(graphics, indices),
		children_(std::move(children))
	{
	}

	void Mesh::Draw(const XMMATRIX& world) const
	{
		auto cb_vs_scene = graphics_->GetCBVSMesh();

		cb_vs_scene.data.world = world;
		
		cb_vs_scene.Bind();
		vertex_buffer_.Bind();
		index_buffer_.Bind();

		graphics_->GetDeviceContext()->DrawIndexed(
			index_buffer_.NumIndices(), 0, 0);
	}

}