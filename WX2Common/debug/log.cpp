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

		// ���O���i�[����f�B���N�g��
		const fs::path dir(FileDirectory);
		WX2_ASSERT_MSG(fs::is_directory(dir), "�w�肳�ꂽ�f�B���N�g��\"{}\"�͑��݂��܂���B");

		// �f�B���N�g�����̃t�@�C�������܂������ɕۂ�
		std::vector log_files(fs::directory_iterator(dir), {});
		if (log_files.size() >= MaxFiles)
		{
			for (const auto& f : vs::counted(log_files.begin(), log_files.size() - (MaxFiles - 1)))
			{
				fs::remove(f);
			}
		}

		// �����̌��ݎ������擾
		auto now = ch::system_clock::now();
		ch::sys_seconds now_sec = ch::floor<ch::seconds>(now);
		ch::zoned_time zoned_time{ "Asia/Tokyo", now_sec };

		// ���O�t�@�C���̐�����p�X
		const fs::path path = dir / std::format(FileName, zoned_time);

		// ���O�̏o�͐���w��
		std::vector<spdlog::sink_ptr> logger_sink;
		logger_sink.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logger_sink.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string(), true));

		// ���K�[���쐬
		logger_ = std::make_shared<spdlog::logger>("logger", logger_sink.begin(), logger_sink.end());
		logger_->set_level(spdlog::level::trace);
		logger_->flush_on(spdlog::level::trace);

		// �f�t�H���g�Ɏw�肵�ČĂяo����e�Ղɂ���
		set_default_logger(logger_);
	}

	logger::~logger()
	{
		// ���K�[���I��������
		spdlog::shutdown();
	}
}