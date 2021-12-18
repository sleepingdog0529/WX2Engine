/*********************************************************************
 * @file	scene.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 16:28
 * @brief	�V�[��
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
		
		WX2_DISALLOW_COPY(Scene);
		WX2_DISALLOW_MOVE(Scene);

		/**
		 * @brief �V�[���̊J�n���̏���
		 * @param parameter �V�[��
		 */
		void Start(const std::string& sceneName, const SceneParameter& parameter = {});
		//! @brief �V�[���X�V���̏���
		void Update();
		//! @brief �V�[���`�掞�̏���
		void Draw();
	};
}