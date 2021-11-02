/*********************************************************************
 * @file	string_helper.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 2:57
 * @brief	文字列型のヘルパー
*********************************************************************/
#pragma once

namespace wx2
{
	/**
	 * @brief	  string_viewをstringに変換する
	 * @param[in] strv 文字列ビュー
	 * @return    文字列
	 */
	inline std::string ToString(std::string_view strv) 
	{
		return { strv.data(), strv.size() };
	}
}