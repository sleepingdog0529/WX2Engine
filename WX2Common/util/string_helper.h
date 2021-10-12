/*********************************************************************
 * @file	string_helper.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 2:57
 * @brief	文字列型のヘルパー
*********************************************************************/
#pragma once

namespace wx2
{
	/// <summary>
	/// string_viewをstringに変換
	/// </summary>
	/// <param name="strv">文字列ビュー</param>
	/// <returns>文字列</returns>
	inline std::string ToString(std::string_view strv) {
		return { strv.data(), strv.size() };
	}
}