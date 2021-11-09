/*********************************************************************
 * @file   hash.h
 * @author Tomomi Murakami
 * @date   2021/11/09 23:57
 * @brief  ハッシュ関数オブジェクト
 ********************************************************************/
#pragma once

namespace wx2
{
	struct StringHash {
		using is_transparent = void;

		std::size_t operator()(std::string_view strv) const {
			std::hash<std::string_view> hasher;
			return hasher(strv);
		}
	};
}