/*********************************************************************
 * @file   hash.h
 * @author Tomomi Murakami
 * @date   2021/11/09 23:57
 * @brief  ハッシュ関数オブジェクト
 ********************************************************************/
#pragma once

namespace wx2
{
	/**
	 * @brief 文字列のハッシュ値を求める関数オブジェクト
	 */
	struct StringHash {
		using is_transparent = void;

		/**
		 * @brief     文字列のハッシュ値を求める
		 * @param[in] 文字列ビュー
		 * @return    ハッシュ値
		 */
		std::size_t operator()(std::string_view strv) const {
			std::hash<std::string_view> hasher;
			return hasher(strv);
		}
	};
}