/*********************************************************************
 * @file   com_exception.h
 * @author Tomomi Murakami
 * @date   2021/11/23 02:01
 * @brief  COM�C���^�[�t�F�[�X�̗�O
 ********************************************************************/
#pragma once
#include <format>
#include <source_location>
#include <comdef.h>

/**
 * @brief ����COM�C���^�[�t�F�[�X�̏������ʂ��G���[�ł������Ȃ��O���X���[����
 * @param hr COM�C���^�[�t�F�[�X�̏�������
 * @param __VA_ARGS__ �t�H�[�}�b�g���b�Z�[�W
 */
#define WX2_COM_ERROR_IF_FAILED(hr, ...)	\
	do										\
	{										\
		if (FAILED(hr))						\
		{									\
			throw wx2::COMException(		\
				hr,							\
				std::format(__VA_ARGS__));	\
		}									\
	} while(0)

namespace wx2
{
	class COMException
	{
	public:
		/**
		 * @brief COM�C���^�[�t�F�[�X�̗�O�̏�����
		 * @param hr COM�C���^�[�t�F�[�X�̏�������
		 * @param msg �G���[���
		 * @param location �\�[�X��̃G���[�����ʒu ���K���f�t�H���g�������g�p���邱��
		 */
		COMException(
			const HRESULT hr,
			const std::string& msg,
			const std::source_location& location = std::source_location::current()) noexcept
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
		 * @return ��O�̏ڍ׏��̕�����
		 */
		[[nodiscard]] const std::string& what() const noexcept
		{
			return what_;
		}

	private:
		//! ��O�̏ڍ�
		std::string what_{};
	};
}