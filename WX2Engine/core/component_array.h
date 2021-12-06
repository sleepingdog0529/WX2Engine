/*********************************************************************
 * @file   component_array.h
 * @author Tomomi Murakami
 * @date   2021/12/04 21:30
 * @brief  コンポーネントのリスト
 ********************************************************************/
#pragma once
#include "entity.h"

namespace wx2
{

	class IComponntArray
	{
	public:
		IComponentArray() = default;
		virtual ~IComponentArray() = default;

		WX2_DISALLOW_COPY_AND_MOVE(IComponentArray);

		virtual void EntityDestroyed(const Entity entity) noexcept = 0;
	};

	template <class T>
	class ComponentArray final : public IComponentArray
	{
	public:
		ComponentArray() = default;
		~ComponentArray() override = default;

		WX2_DISALLOW_COPY_AND_MOVE(ComponentArray);

		void InsertData(const Entity entity, const T& component) noexcept
		{
			WX2_ASSERT_MSG(
				!entityToIndexMap_.contains(entity),
				"このエンティティには既に同じコンポーネントが存在します。");

			// Put new entry at end and update the maps
			const std::size_t newIndex = size_;
			entityToIndexMap_[entity] = newIndex;
			indexToEntityMap_[newIndex] = entity;
			components_[newIndex] = component;
			++size_;
		}

		void RemoveData(const Entity entity) noexcept
		{
			WX2_ASSERT_MSG(
				entityToIndexMap_.contains(entity),
				"削除対象のコンポーネントが存在していません。");

			// Copy element at end into deleted element's place to maintain density
			const std::size_t indexOfRemovedEntity = entityToIndexMap_[entity];
			const std::size_t indexOfLastElement = size_ - 1;
			components_[indexOfRemovedEntity] = components_[indexOfLastElement];

			// Update map to point to moved spot
			const Entity entityOfLastElement = indexToEntityMap_[indexOfLastElement];
			entityToIndexMap_[entityOfLastElement] = indexOfRemovedEntity;
			indexToEntityMap_[indexOfRemovedEntity] = entityOfLastElement;

			entityToIndexMap_.erase(entity);
			indexToEntityMap_.erase(indexOfLastElement);

			--size_;
		}

		[[nodiscard]] const T& GetData(const Entity entity) noexcept
		{
			WX2_ASSERT_MSG(
				entityToIndexMap_.contains(entity),
				"削除対象のコンポーネントが存在していません。");

			// Return a reference to the entity's component
			return components_[entityToIndexMap_[entity]];
		}

		void EntityDestroyed(const Entity entity) noexcept override
		{
			// エンティティが格納されていたらデータを消す
			if (entityToIndexMap_.contains(entity))
			{
				RemoveData(entity);
			}
		}

	private:
		std::array<T, MAX_ENTITIES> components_;

		// Map from an entity ID to an array index.
		std::unordered_map<Entity, size_t> entityToIndexMap_;

		// Map from an array index to an entity ID.
		std::unordered_map<size_t, Entity> indexToEntityMap_;

		// Total size of valid entries in the array.
		size_t size_;
	};
}
