/*********************************************************************
 * @file   file_dialog.h
 * @author Tomomi Murakami
 * @date   2021/12/03 20:33
 * @brief  ファイル選択ダイアログ
 ********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <filesystem>
#pragma warning(pop)

namespace wx2
{
	std::filesystem::path OpenFileDialog(const std::string& filter);
	std::filesystem::path SaveFileDialog(const std::string& filter);
}
