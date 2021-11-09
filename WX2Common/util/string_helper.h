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
	 * @return    変換後の文字列
	 */
	inline std::string ToString(std::string_view strv) 
	{
		return { strv.data(), strv.size() };
	}

	/**
	 * @brief	  wstringをstringに変換する
	 * @param[in] wstr ワイド文字列
	 * @return    変換後の文字列
	 */
	inline std::string ToString(const std::wstring& wstr)
	{
		std::size_t len = wstr.size();
		std::size_t converted_len;

		char* conv = static_cast<char*>(malloc(sizeof(char) * len * 2 + 1));
		errno_t error = wcstombs_s(&converted_len, conv, sizeof(char) * len * 2 + 1, wstr.c_str(), len * 2);
		if (error != 0 || conv == nullptr)
		{
			WX2_LOG_ERROR("ワイド文字列から文字列への変換に失敗しました。");
			exit(EXIT_FAILURE);
		}

		std::string conv_str(conv);
		free(conv);
		return conv_str;
	}
}