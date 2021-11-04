/*********************************************************************
 * @file	vertex.h
 * @author	SleepingDog0529
 * @date	2021/07/11 14:38
*********************************************************************/
#pragma once

#include <Eigen/Dense>

namespace wx2 {

	struct Vertex3D
	{
		XMFLOAT3 position;
		XMFLOAT2 texcoord;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};

}