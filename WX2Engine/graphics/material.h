/*********************************************************************
 * @file	material.h
 * @author	SleepingDog0529
 * @date	2021/09/02 16:22
*********************************************************************/
#pragma once
#include "type/color4.h"

namespace wx2
{

	struct Material
	{
		Color4 ambient_color;
		Color4 diffuse_color;
		Color4 specular_color;
		Color4 emissive_color;
		float  ambient_factor;
		float  diffuse_factor;
		float  specular_factor;
		float  emissive_factor;
	};

}