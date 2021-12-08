/*********************************************************************
 * @file	scene.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 16:28
 * @brief	シーンベース
 ********************************************************************/
#pragma once
#include "scene_parameter.h"

namespace wx2
{
	class SceneBase
	{
	public:
		SceneBase() = default;
		virtual ~SceneBase() = default;

		virtual void Start(const SceneParameter& parameter = {}) = 0;
		void Update();
		void Draw();

	private:
	};
}