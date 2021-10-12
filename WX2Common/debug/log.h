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
#include "../util/macro.h"

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
	class Logger
	{
	private:
		static constexpr std::size_t MAX_FILE_ = 3;	// ���O�t�@�C���̍ő吔
		static constexpr const char* FILE_DIR_ = ".\\log";	// ���O�t�@�C����u���f�B���N�g��
		static constexpr const char* FILE_NAME_ = "{:%Y-%m-%d-%H-%M-%S}.log";	// ���O�t�@�C�����̃t�H�[�}�b�g

	public:
		Logger();
		virtual ~Logger();

		WX2_DISALLOW_COPY_AND_MOVE(Logger);

	private:
		std::shared_ptr<spdlog::logger> logger_;
	};
}