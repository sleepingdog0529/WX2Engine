/*********************************************************************
 * @file   model_storage.h
 * @author Tomomi Murakami
 * @date   2021/12/20 12:10
 * @brief  3Dモデルストレージ
 ********************************************************************/
#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "drawable.h"

namespace wx2
{
	class ModelStorage final
	{
	public:
		ModelStorage() = default;
		~ModelStorage() = default;

		WX2_COPYABLE(ModelStorage);
		WX2_MOVEABLE(ModelStorage);

		void Insert(const std::string& key, std::shared_ptr<Drawable> model)
		{
			models_[key] = std::move(model);
		}

		void Erase(const std::string& key)
		{
			models_.erase(key);
		}

		std::shared_ptr<Drawable> Find(const std::string& key)
		{
			const auto itr = models_.find(key);

			if (itr == models_.end())
				return nullptr;
			return itr->second;
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Drawable>> models_;
	};
}
