/*********************************************************************
 * @file	model.h
 * @author	SleepingDog0529
 * @date	2021/07/15 17:35
*********************************************************************/
#pragma once
#include "mesh.h"

namespace wx2 {

	class Model
	{
	public:
		Model(Graphics* graphics, std::vector<Mesh>& meshes);
		virtual ~Model() = default;

		WX2_DISALLOW_COPY_AND_MOVE(Model);

		void Draw(const XMMATRIX& world);

	private:
		Graphics* graphics_;
		std::vector<Mesh> meshes_;
	};

}