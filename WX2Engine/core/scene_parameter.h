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
	/**
	 * @brief  シーン間で情報伝達するためのパラメータークラス
	 */
	class SceneParameter final
	{
	private:
		// パラメーターとして設定できる値型
		using ParameterType = std::variant<int, std::string>;

	public:
		SceneParameter() = default;
		~SceneParameter() = default;

		// コピーとムーブ禁止
		WX2_DISALLOW_COPY_AND_MOVE(SceneParameter);

		/**
		 * @brief  パラメーターに値を追加する
		 * @tparam T パラメーター値型
		 * @param  key パラメーター名
		 * @param  param パラメーターの値
		 */
		template <typename T>
		void Add(const std::string& key, const T& param) noexcept
		{
			parameters_[key] = param;
		}

		/**
		 * @brief  パラメーターから値を取得する
		 * @tparam T 取得する値の型
		 * @param  key 取得するパラメーター名
		 * @return パラメーターの値、存在しなければnullopt
		 */
		template <typename T>
		std::optional<T> Get(const std::string& key) const noexcept
		{
			// 指定されたキーが見つらないか、値の型が違ったらnulloptを返す
			const auto itr = parameters_.find(key);
			if (itr == parameters_.end() || 
				!std::holds_alternative<T>(itr->second))
			{
				return std::nullopt;
			}

			// 指定された値型に変換して返す
			return std::get<T>(itr->second);
		}

	private:
		//! パラメータのマップ
		std::unordered_map<std::string, ParameterType> parameters_{};
	};
}
