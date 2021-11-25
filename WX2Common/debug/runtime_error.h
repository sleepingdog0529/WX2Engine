/*********************************************************************
 * @file   exception.h
 * @author Tomomi Murakami
 * @date   2021/11/25 00:13
 * @brief  例外
 ********************************************************************/
#pragma once
#include <format>
#include <source_location>

/**
 * @brief もし判定式がfalseであったなら例外をスローする
 * @param expr 判定式
 */
#define WX2_RUNTIME_ERROR_IF_FAILED(expr, ...)				\
	if (!expr)												\
	{														\
		throw wx2::RuntimeError(std::format(__VA_ARGS__));	\
	}

namespace wx2
{
	class RuntimeError
	{
	public:
		/**
		 * @brief 実行時エラーの初期化
		 * @param msg エラー情報
		 * @param location ソース上のエラー発生位置 ※必ずデフォルト引数を使用すること
		 */
		explicit RuntimeError(
			const std::string& msg,
			const std::source_location& location = std::source_location::current())
		{
			what_ = std::format(
				"\n"
				"==============< Exception thrown ! >==============\n"
				"Message: {}\n"
				"File: {}\n"
				"Function: {}\n"
				"Line: {}\n"
				"==================================================\n",
				msg,
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
