/*********************************************************************
 * @file   exception.h
 * @author Tomomi Murakami
 * @date   2021/11/25 00:13
 * @brief  ��O
 ********************************************************************/
#pragma once
#include <format>
#include <source_location>

/**
 * @brief �������莮��false�ł������Ȃ��O���X���[����
 * @param expr ���莮
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
		 * @brief ���s���G���[�̏�����
		 * @param msg �G���[���
		 * @param location �\�[�X��̃G���[�����ʒu ���K���f�t�H���g�������g�p���邱��
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
		 * @return ��O�̏ڍ׏��̕�����
		 */
		[[nodiscard]] const std::string& what() const
		{
			return what_;
		}

	private:
		//! ��O�̏ڍ�
		std::string what_{};
	};
}
