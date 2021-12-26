/*********************************************************************
 * @file   flyweight.h
 * @author Tomomi Murakami
 * @date   2021/12/21 21:57
 * @brief  フライウェイトパターン
 ********************************************************************/
#pragma once
#include <unordered_map>
#include <memory>
#include "macro.h"

namespace wx2
{
	namespace detail
	{
		struct NoTag{};
	}

	template <
		typename KeyType,
		typename ValueType,
		typename Tag = detail::NoTag>
	class Flyweight
	{
	public:
		Flyweight() = default;
		virtual ~Flyweight() = default;

		WX2_COPYABLE(Flyweight);
		WX2_MOVEABLE(Flyweight);

		template <typename... Args>
		std::shared_ptr<ValueType> Emplace(const KeyType& key, Args&&... args)
		{
			if (const auto itr = pool_.find(key); itr != pool_.end())
			{
				return itr->second;
			}

			const auto [itr, succsess] = pool_.emplace(
				key,
				std::make_shared<ValueType>(std::forward<Args>(args)...));

			return itr->second;
		}
		
		std::shared_ptr<ValueType> Get(const KeyType& key)
		{
			if (const auto itr = pool_.find(key); itr != pool_.end())
			{
				return itr->second;
			}

			return nullptr;
		}
		
	private:
		//! 
		static inline std::unordered_map<KeyType, std::shared_ptr<ValueType>> pool_;
	};
}