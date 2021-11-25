/*********************************************************************
 * @file   com_exception.h
 * @author Tomomi Murakami
 * @date   2021/11/23 02:01
 * @brief  COMインターフェースの例外
 ********************************************************************/
#pragma once
#include <format>
#include <source_location>
#include <comdef.h>

/**
 * @brief もし処理結果がエラーであったなら例外をスローする
 * @param hr COMインターフェースの処理結果
 */
#define WX2_COM_ERROR_IF_FAILED(hr, ...)		\
	if (FAILED(hr))								\
	{											\
		throw wx2::COMException(				\
			hr,									\
			std::format(__VA_ARGS__));			\
	}

namespace wx2
{
	class COMException
	{
	public:
		/**
		 * @brief COMインターフェースの例外の初期化
		 * @param hr COMインターフェースの処理結果
		 * @param msg エラー情報
		 * @param location ソース上のエラー発生位置 ※必ずデフォルト引数を使用すること
		 */
		COMException(
			const HRESULT hr,
			const std::string& msg,
			const std::source_location& location = std::source_location::current())
		{
			const _com_error error(hr);

			what_ = std::format(
				"\n"
				"==============< Exception thrown ! >==============\n"
				"Message: {}\n"
				"Error: {}\n"
				"File: {}\n"
				"Function: {}\n"
				"Line: {}\n"
				"==================================================\n",
				msg,
				error.ErrorMessage(),
				location.file_name(),
				location.function_name(),
				location.line());
		}

		/**
		 * @return 例外の詳細情報の文字列
		 */
		[[nodiscard]] const std::string& what() const
		{
			return what_;
		}

	private:
		//! 例外の詳細
		std::string what_{};
	};
}