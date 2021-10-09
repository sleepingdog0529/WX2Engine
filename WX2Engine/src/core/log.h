/*********************************************************************
 * @file	log.h
 * @author	Tomomi Murakami
 * @date	2021/09/18 22:35
 * @brief	���O���o�́E�L�^����
*********************************************************************/
#pragma once
#pragma warning(push, 0) 
#include <spdlog/spdlog.h>
#pragma warning(pop)

#define WX2_LOG_TRACE(...)    spdlog::trace(__VA_ARGS__)
#define WX2_LOG_INFO(...)     spdlog::info(__VA_ARGS__)
#define WX2_LOG_WARN(...)     spdlog::warn(__VA_ARGS__)
#define WX2_LOG_ERROR(...)    spdlog::error(__VA_ARGS__)
#define WX2_LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)

namespace wx2
{
	/// <summary>
	/// ���M���O�N���X
	/// </summary>
	class logger
	{
	private:
		static constexpr size_t MaxFiles = 3;	// ���O�t�@�C���̍ő吔
		static constexpr const char* FileDirectory = ".\\log";	// ���O�t�@�C����u���f�B���N�g��
		static constexpr const char* FileName = "{:%Y-%m-%d-%H-%M-%S}.log";	// ���O�t�@�C�����̃t�H�[�}�b�g

	public:
		logger();
		virtual ~logger();

		WX2_DISALLOW_COPY_AND_MOVE(logger);

	private:
		std::shared_ptr<spdlog::logger> logger_;
	};
}