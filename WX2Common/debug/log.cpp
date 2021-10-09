#include "log.h"
#include <filesystem>
#include <ranges>
#include <chrono>
#include "assert.h"
#pragma warning(push, 0) 
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

namespace wx2
{
	logger::logger()
	{
		namespace fs = std::filesystem;
		namespace vs = std::views;
		namespace ch = std::chrono;

		// ログを格納するディレクトリ
		const fs::path dir(FileDirectory);
		WX2_ASSERT_MSG(fs::is_directory(dir), "指定されたディレクトリ\"{}\"は存在しません。");

		// ディレクトリ内のファイルを決まった数に保つ
		std::vector log_files(fs::directory_iterator(dir), {});
		if (log_files.size() >= MaxFiles)
		{
			for (const auto& f : vs::counted(log_files.begin(), log_files.size() - (MaxFiles - 1)))
			{
				fs::remove(f);
			}
		}

		// 東京の現在時刻を取得
		auto now = ch::system_clock::now();
		ch::sys_seconds now_sec = ch::floor<ch::seconds>(now);
		ch::zoned_time zoned_time{ "Asia/Tokyo", now_sec };

		// ログファイルの生成先パス
		const fs::path path = dir / std::format(FileName, zoned_time);

		// ログの出力先を指定
		std::vector<spdlog::sink_ptr> logger_sink;
		logger_sink.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logger_sink.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string(), true));

		// ロガーを作成
		logger_ = std::make_shared<spdlog::logger>("logger", logger_sink.begin(), logger_sink.end());
		logger_->set_level(spdlog::level::trace);
		logger_->flush_on(spdlog::level::trace);

		// デフォルトに指定して呼び出しを容易にする
		set_default_logger(logger_);
	}

	logger::~logger()
	{
		// ロガーを終了させる
		spdlog::shutdown();
	}
}