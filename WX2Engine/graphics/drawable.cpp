//#include "drawable.h"
//
//namespace WX2 
//{
//
//	Drawable::Drawable(
//		ID3D11Device* device, 
//		ID3D11DeviceContext* device_context,
//		ConstantBuffer<CBVSMesh>* cbvs_par_object,
//		std::vector<Vertex3D>& vertices,
//		std::vector<UINT>& indices) :
//		device_context_(device_context),
//		cbvs_par_object_(cbvs_par_object)
//	{
//		vertex_buffer_ = std::make_unique<VertexBuffer<Vertex3D>>(device, std::move(vertices));
//		index_buffer_ = std::make_unique<IndexBuffer>(device, std::move(indices));
//	}
//
//	void Drawable::Draw(
//		const XMMATRIX& world_matrix,
//		const XMMATRIX& view_matrix,
//		const XMMATRIX& projection_matrix)
//	{
//		cbvs_par_object_->Data.WorldMatrix = world_matrix;
//		cbvs_par_object_->Data.ViewMatrix = view_matrix;
//		cbvs_par_object_->Data.ProjectionMatrix = projection_matrix;
//		cbvs_par_object_->ApplyChanges();
//
//		device_context_->VSSetConstantBuffers(0, 1, cbvs_par_object_->GetAddressOf());
//
//		const UINT offset = 0;
//		device_context_->IASetVertexBuffers(0, 1, vertex_buffer_->GetAddressOf(), vertex_buffer_->StridePtr(), &offset);
//		device_context_->IASetIndexBuffer(index_buffer_->Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
//
//		device_context_->DrawIndexed(index_buffer_->NumIndices(), 0, 0);
//	}
//
//}