/*********************************************************************
 * @file	scene.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 16:28
 * @brief	シーン
 ********************************************************************/
#pragma once
#include "scene_parameter.h"

namespace wx2
{
	class Scene final
	{
	public:
		Scene() = default;
		~Scene() = default;

		// コピーとムーブ禁止
		WX2_DISALLOW_COPY_AND_MOVE(Scene);

		/**
		 * @brief シーンの開始時の処理
		 * @param parameter シーン
		 */
		void Start(const std::string& sceneName, const SceneParameter& parameter = {});
		//! @brief シーン更新時の処理
		void Update();
		//! @brief シーン描画時の処理
		void Draw();
	};
}