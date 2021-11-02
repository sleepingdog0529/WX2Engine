/*********************************************************************
 * @file	log.h
 * @author	Tomomi Murakami
 * @date	2021/09/18 22:35
 * @brief	ログを出力・記録する
 ********************************************************************/
#pragma once
#pragma warning(push, 0) 
#include <spdlog/spdlog.h>
#pragma warning(pop)
#include "../util/macro.h"

/**
 * @brief	  ログを出力する
 * @param[in] フォーマット書式文字列、引数
 */
#define WX2_LOG_TRACE(...)    spdlog::trace(__VA_ARGS__)
#define WX2_LOG_INFO(...)     spdlog::info(__VA_ARGS__)
#define WX2_LOG_WARN(...)     spdlog::warn(__VA_ARGS__)
#define WX2_LOG_ERROR(...)    spdlog::error(__VA_ARGS__)
#define WX2_LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)

namespace wx2
{
	/**
	 * @brief ログの管理を行うクラス
	 */
	class Logger
	{
	private:
		//! ログファイルの最大数
		static constexpr std::size_t MAX_FILE_ = 3; 
		//! ログファイルを置くディレクトリ
		static constexpr const char* FILE_DIR_ = ".\\log";	
		//! ログファイル名のフォーマット
		static constexpr const char* FILE_NAME_ = "{:%Y-%m-%d-%H-%M-%S}.log";	

	public:
		/**
		 * @brief ロガーの初期化、ファイルの数制御を行う
		 */
		Logger();

		/**
		 * @brief ロガーの終了処理
		 */
		virtual ~Logger();

		WX2_DISALLOW_COPY_AND_MOVE(Logger);

	private:
		//! ロガー本体
		std::shared_ptr<spdlog::logger> logger_;
	};
}