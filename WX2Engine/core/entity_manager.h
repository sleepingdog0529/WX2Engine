/*********************************************************************
 * @file   entity_manager.h
 * @author Tomomi Murakami
 * @date   2021/12/04 18:06
 * @brief  ECSのエンティティマネージャー
 ********************************************************************/
#pragma once
#include "entity.h"

namespace wx2
{

	/**
	 * @brief  エンティティIDとシグネチャを管理する
	 */
	class EntityManager
	{
	private:
		//! コンポーネントの最大数
		static constexpr std::size_t MAX_COMPONENTS = 32;

		//! エンティティが持つコンポーネントIDをビット列で保持する
		using Signature = std::bitset<MAX_COMPONENTS>;

	public:
		EntityManager() noexcept;
		virtual ~EntityManager() = default;

		WX2_DISALLOW_COPY_AND_MOVE(EntityManager);

		/**
		 * @brief  エンティティを生成する
		 * @return 生成したエンティティID
		 */
		Entity CreateEntity();

		/**
		 * @brief  エンティティを削除する
		 * @param  entity 削除するエンティティID
		 */
		void DestroyEntity(const Entity entity);

		/**
		 * @brief  エンティティにシグネチャをセットする
		 * @param  entity セット先のエンティティID
		 * @param  signature エンティティにセットするシグネチャ
		 */
		void SetSignature(const Entity entity, const Signature& signature);

		/**
		 * @brief  エンティティのシグネチャを取得する
		 * @param  entity 取得先のエンティティID
		 * @return エンティティのシグネチャ
		 */
		[[nodiscard]] Signature GetSignature(const Entity entity) const noexcept;

	private:
		//! 使用していないエンティティIDのリスト
		std::queue<Entity> availableEntities_{};

		//! エンティティのシグネチャ配列
		std::array<Signature, MAX_ENTITIES> signatures_{};

		//! 使用されているエンティティの数
		uint32_t livingEntityCount_{};
	};
}
