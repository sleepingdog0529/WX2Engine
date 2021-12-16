/*********************************************************************
 * @file   scene_parameter.h
 * @author Tomomi Murakami
 * @date   2021/12/04 17:39
 * @brief  シーンへ渡すパラメーター
 ********************************************************************/
#pragma once
#include <any>
#include <string>
#include <unordered_map>

namespace wx2
{
	/**
	 * @brief  シーン間で情報伝達するためのパラメータークラス
	 */
	class SceneParameter final
	{
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
			try
			{
				// 指定されたキーの値を取得する
				const auto& val = parameters_.at(key);

				// 指定された値型に変換して返す
				return std::any_cast<T>(val);
			}
			catch (const std::out_of_range& exception)
			{
				WX2_LOG_ERROR("シーンパラメーターのキーを持つ要素が存在しません");
				return std::nullopt;
			}
			catch (const std::bad_any_cast& exception)
			{
				WX2_LOG_ERROR("シーンパラメーターを間違った型で取得しようとしました。");
				return std::nullopt;
			}
		}

	private:
		//! パラメータのマップ
		std::unordered_map<std::string, std::any> parameters_{};
	};
}
