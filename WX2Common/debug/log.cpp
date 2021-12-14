#include "log.h"
#include <filesystem>
#include <ranges>
#include <chrono>
#pragma warning(push, 0) 
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

namespace wx2
{
	Logger::Logger() noexcept
	{
		namespace fs = std::filesystem;
		namespace vs = std::views;
		namespace ch = std::chrono;

		// ログを格納するディレクトリ
		const fs::path dir(FILE_DIR);
		if (!fs::is_directory(dir))
		{
			WX2_LOG_INFO("指定されたログ格納用のディレクトリは存在しません。新たに作成します。パス: {}", dir.string());
			fs::create_directory(dir);
		}

		// ディレクトリ内のログファイルを決まった数に保つ
		if (std::vector logFiles(fs::directory_iterator(dir), {}); logFiles.size() >= MAX_FILE)
		{
			// 指定数よりログファイルが多ければ古い順に消す
			for (const auto& f : vs::counted(logFiles.begin(), logFiles.size() - (MAX_FILE - 1)))
			{
				fs::remove(f);
			}
		}

		// 東京の現在時刻を取得
		const auto now = ch::system_clock::now();
		const ch::sys_seconds nowSec = ch::floor<ch::seconds>(now);
		const ch::zoned_time zonedTime{ "Asia/Tokyo", nowSec };

		// ログファイルの生成先パス
		const fs::path path = dir / std::format(FILE_NAME, zonedTime);

		// ログの出力先を設定
		std::vector<spdlog::sink_ptr> loggerSink;
		loggerSink.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		loggerSink.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string(), true));

		// ロガーを作成
		logger_ = std::make_shared<spdlog::logger>("logger", loggerSink.begin(), loggerSink.end());
		logger_->set_level(spdlog::level::trace);
		logger_->flush_on(spdlog::level::trace);

		// デフォルトに指定して呼び出しを容易にする
		set_default_logger(logger_);
	}

	Logger::~Logger() noexcept
	{
		// ロガーを終了させる
		spdlog::shutdown();
	}
}