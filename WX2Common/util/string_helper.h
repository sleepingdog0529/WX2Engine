/*********************************************************************
 * @file   string_helper.h
 * @author Tomomi Murakami
 * @date   2021/09/19 2:57
 * @brief  文字列型のヘルパー
*********************************************************************/
#pragma once
#include <string>
#include <cstdlib>

namespace wx2
{
	/**
	 * @brief  string_viewをstringに変換する
	 * @param  strv 文字列ビュー
	 * @return 変換後の文字列
	 */
	inline std::string ToString(std::string_view strv) noexcept
	{
		return { strv.data(), strv.size() };
	}

	/**
	 * @brief  マルチバイト文字列をワイド文字列に変換する
	 * @param  str 変換対象のマルチバイト文字列
	 * @return 変換されたワイド文字列
	 */
	inline std::wstring Utf8ToUtf16(const std::string_view str) noexcept
	{
		// 変換対象の文字列が空の場合
		if (str.empty() || str[0] == '\0')
			return {};

		// 変換に必要なサイズを確保
		std::wstring out(str.size() * sizeof(char), '\0');

		// ワイド文字列への変換
		if (::mbstowcs_s(nullptr, out.data(), out.size() + 1, str.data(), _TRUNCATE) != 0)
			return {};

		// 終端から後ろの不要な部分を削る
		out.resize(std::char_traits<wchar_t>::length(out.data()));

		return out;
	}

	/**
	 * @brief  ワイド文字列をマルチバイト文字列に変換する
	 * @param  wstr 変換対象のワイド文字列
	 * @return 変換されたマルチバイト文字列
	 */
	inline std::string Utf16ToUtf8(const std::wstring_view wstr) noexcept
	{
		// 変換対象の文字列が空の場合
		if (wstr.empty() || wstr[0] == L'\0')
			return {};

		// 変換に必要なサイズを確保
		std::string out(wstr.size() * sizeof(wchar_t), '\0');

		// マルチバイト文字列への変換
		if (::wcstombs_s(nullptr, out.data(), out.size() + 1, wstr.data(), _TRUNCATE) != 0)
			return {};

		// 終端から後ろの不要な部分を削る
		out.resize(std::char_traits<char>::length(out.data()));

		return out;
	}
}