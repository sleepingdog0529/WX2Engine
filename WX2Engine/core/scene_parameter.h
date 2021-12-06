/*********************************************************************
 * @file   scene_parameter.h
 * @author Tomomi Murakami
 * @date   2021/12/04 17:39
 * @brief  シーンへ渡すパラメーター
 ********************************************************************/
#pragma once
#include <variant>
#include <string>
#include <unordered_map>

namespace wx2
{
	class SceneParameter final
	{
	private:
		using ParameterType = std::variant<int, std::string>;

	public:
		SceneParameter() = default;
		~SceneParameter() = default;

		WX2_DISALLOW_COPY_AND_MOVE(SceneParameter);

		template <typename T>
		void Add(const std::string& key, const T& param) noexcept
		{
			parameters_[key] = param;
		}

		template <typename T>
		std::optional<T> Get(const std::string& key) const noexcept
		{
			const auto itr = parameters_.find(key);
			if (itr == parameters_.end() || 
				!std::holds_alternative<T>(itr->second))
			{
				return std::nullopt;
			}

			return std::get<T>(itr->second);
		}

	private:
		std::unordered_map<std::string, ParameterType> parameters_{};
	};
}
