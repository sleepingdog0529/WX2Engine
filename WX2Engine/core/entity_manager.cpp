#include "entity_manager.h"

namespace wx2
{
	EntityManager::EntityManager() noexcept
	{
		// 利用可能なエンティティIDを初期化
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			availableEntities_.push(entity);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		WX2_RUNTIME_ERROR_IF_FAILED(
			livingEntityCount_ < MAX_ENTITIES, "エンティティが多すぎます。");

		// 未使用エンティティIDをポップして使用
		const Entity id = availableEntities_.front();
		availableEntities_.pop();
		++livingEntityCount_;

		return id;
	}

	void EntityManager::DestroyEntity(const Entity entity)
	{
		WX2_ASSERT_MSG(entity < MAX_ENTITIES, "エンティティIDが範囲外です。");

		// エンティティが消されて必要なくなったのでシグネチャをリセット
		signatures_[entity].reset();

		// 未使用エンティティIDに加える
		availableEntities_.push(entity);
		--livingEntityCount_;
	}

	void EntityManager::SetSignature(const Entity entity, const Signature& signature)
	{
		WX2_ASSERT_MSG(entity < MAX_ENTITIES, "エンティティIDが範囲外です。");

		// エンティティIDが指すシグネチャを上書き
		signatures_[entity] = signature;
	}

	EntityManager::Signature EntityManager::GetSignature(const Entity entity) const noexcept
	{
		WX2_ASSERT_MSG(entity < MAX_ENTITIES, "エンティティIDが範囲外です。");

		// エンティティIDが指すシグネチャを返す
		return signatures_[entity];
	}
}