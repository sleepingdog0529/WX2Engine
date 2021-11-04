///*********************************************************************
// * @file	drawable.h
// * @author	SleepingDog0529
// * @date	2021/07/15 16:41
//*********************************************************************/
//#pragma once
//#include "vertex.h"
//#include "vertex_buffer.h"
//#include "index_buffer.h"
//#include "constant_buffer_types.h"
//#include "constant_buffer.h"
//
//namespace WX2 
//{
//
//	class Drawable
//	{
//	public:
//		Drawable(
//			ID3D11Device* device, 
//			ID3D11DeviceContext* device_context,
//			ConstantBuffer<CBVSMesh>* cbvs_par_object,
//			std::vector<Vertex3D>& vertices,
//			std::vector<UINT>& indices);
//		virtual ~Drawable() = default;
//
//		WX2_DISALLOW_COPY_AND_MOVE(Drawable);
//
//		void Draw(
//			const XMMATRIX& world_matrix,
//			const XMMATRIX& view_matrix,
//			const XMMATRIX& projection_matrix);
//
//	private:
//		ID3D11DeviceContext* device_context_;
//		ConstantBuffer<CBVSMesh>* cbvs_par_object_;
//		VertexBuffer<Vertex3D> vertex_buffer_;
//		IndexBuffer index_buffer_;
//	};
//
//}