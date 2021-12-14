/*********************************************************************
 * @file   com_exception.h
 * @author Tomomi Murakami
 * @date   2021/11/23 02:01
 * @brief  実行時の例外
 ********************************************************************/
#pragma once
#include <format>
#include <source_location>

/**
 * @brief もし判定式が偽であったならランタイムエラーをスローする
 * @param expr 判定式
 * @param __VA_ARGS__ フォーマットメッセージ
 */
#define WX2_RUNTIME_ERROR_IF_FAILED(expr, ...)	\
	do											\
	{											\
		if (!(expr))							\
		{										\
			throw wx2::RuntimeError(			\
				#expr,							\
				std::format(__VA_ARGS__));		\
		}										\
	} while(0)

namespace wx2
{
	class RuntimeError
	{
	public:
		/**
		 * @brief ランタイムエラーの初期化
		 * @param expr 判定式文字列
		 * @param msg エラー情報
		 * @param location ソース上のエラー発生位置 ※必ずデフォルト引数を使用すること
		 */
		RuntimeError(
			const std::string& expr,
			const std::string& msg,
			const std::source_location& location = std::source_location::current()) noexcept
		{
			// エラーメッセージ
			what_ = std::format(
				"\n"
				"==============< Exception thrown ! >==============\n"
				"Message: {}\n"
				"Expression: {}\n"
				"File: {}\n"
				"Function: {}\n"
				"Line: {}\n"
				"==================================================\n",
				msg,						// 指定したメッセージ
				expr,						// エラーの判定式
				location.file_name(),		// エラーが発生したファイル名
				location.function_name(),	// エラーが発生した関数名
				location.line());			// エラーが発生した行
		}

		/**
		 * @brief  例外のエラーメッセージを取得
		 * @return エラーメッセージ
		 */
		[[nodiscard]] const std::string& what() const noexcept
		{
			return what_;
		}

	private:
		//! 例外の詳細
		std::string what_{};
	};
}