/*********************************************************************
 * @file	log.h
 * @author	Tomomi Murakami
 * @date	2021/09/18 22:35
 * @brief	ログを出力・記録する
 ********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include "../util/macro.h"
#pragma warning(pop)

/**
 * @brief 追跡ログを出力する
 * @param フォーマット書式メッセージ、引数
 */
#define WX2_LOG_TRACE(...)    spdlog::trace(__VA_ARGS__)
/**
 * @brief 詳細ログを出力する
 * @param フォーマット書式メッセージ、引数
 */
#define WX2_LOG_INFO(...)     spdlog::info(__VA_ARGS__)
/**
 * @brief 警告ログを出力する
 * @param フォーマット書式メッセージ、引数
 */
#define WX2_LOG_WARN(...)     spdlog::warn(__VA_ARGS__)
/**
 * @brief エラーログを出力する
 * @param フォーマット書式メッセージ、引数
 */
#define WX2_LOG_ERROR(...)    spdlog::error(__VA_ARGS__)
/**
 * @brief 重大なエラーのログを出力する
 * @param フォーマット書式メッセージ、引数
 */
#define WX2_LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)

namespace wx2
{
	/**
	 * @brief ログの管理を行うクラス
	 */
	class Logger final
	{
	private:
		//! ログファイルの最大数
		static constexpr std::size_t MAX_FILE = 3; 
		//! ログファイルを置くディレクトリ
		static constexpr const char* FILE_DIR = ".\\log";	
		//! ログファイルの命名フォーマット
		static constexpr const char* FILE_NAME = "{:%Y-%m-%d-%H-%M-%S}.log";	

	public:
		//! @brief ロガーの初期化、ログファイルの管理を行う
		Logger() noexcept;

		//! @brief ロガーの終了処理
		~Logger() noexcept;

		// コピーとムーブ禁止
		WX2_DISALLOW_COPY_AND_MOVE(Logger);

	private:
		//! ロガー本体
		std::shared_ptr<spdlog::logger> logger_;
	};
}