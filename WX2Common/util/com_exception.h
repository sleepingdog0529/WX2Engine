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

#define WX2_COM_ERROR_IF_FAILED(hr, ...)		\
	if (FAILED(hr))								\
	{											\
		const auto location =					\
			std::source_location::current();	\
		throw wx2::ComException(				\
			hr,									\
			std::format(__VA_ARGS__),			\
			location);							\
	}

namespace wx2
{
	class ComException
	{
	public:
		/**
		 * @brief COMインターフェースの例外の初期化
		 * @param hr COMインターフェースの処理結果
		 * @param msg エラーメッセージ
		 * @param location ファイル上でのエラー発生場所
		 */
		ComException(
			const HRESULT hr,
			const std::string& msg,
			const std::source_location& location)
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
		[[nodiscard]] const char* what() const
		{
			return what_.c_str();
		}

	private:
		//! 例外の詳細情報の文字列
		std::string what_;
	};
}