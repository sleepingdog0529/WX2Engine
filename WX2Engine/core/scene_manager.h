/*********************************************************************
 * @file   scene_manager.h
 * @author Tomomi Murakami
 * @date   2021/12/16 13:52
 * @brief  シーン管理
 ********************************************************************/
#pragma once
#include "scene.h"
#include "scene_parameter.h"

namespace wx2
{
	class SceneStack final
	{
	public:
		SceneStack() = default;
		~SceneStack() = default;

		WX2_DISALLOW_COPY(SceneStack);
		WX2_DISALLOW_MOVE(SceneStack);

		/**
		 * @brief  シーンをプッシュする
		 * @tparam SceneType プッシュするシーンの型
		 * @param  sceneParameter 
		 * @param  clear 
		 * @return 
		 */
		template<class SceneType>
		std::shared_ptr<SceneType> PushScene(const SceneParameter& sceneParameter, const bool clear)
		{
			// シーンスタックをクリアする
			if (clear)
			{
				while (!scenes_.empty())
				{
					scenes_.pop();
				}
			}
			scenes_.emplace(std::make_shared<SceneType>(sceneParameter));
		}

	private:
		std::stack<std::shared_ptr<SceneBase>> scenes_;
	};
}
