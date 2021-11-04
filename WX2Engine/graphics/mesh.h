/*********************************************************************
 * @file	mesh.h
 * @author	SleepingDog0529
 * @date	2021/06/23 12:25
*********************************************************************/
#pragma once
#include "graphics.h"
#include "vertex.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

namespace wx2 {

	class Mesh
	{
	public:
		Mesh(Graphics* graphics,
			std::span<Vertex3D> vertices,
			std::span<UINT> indices,
			std::vector<Mesh>& children);
		virtual ~Mesh() = default;

		WX2_DISALLOW_COPY_AND_MOVE(Mesh);

		void Draw(const XMMATRIX& world) const;

	private:

		Graphics* graphics_;
		XMMATRIX transform_matrix_;
		VertexBuffer<Vertex3D> vertex_buffer_;
		IndexBuffer index_buffer_;

		std::vector<Mesh> children_;
	};

}