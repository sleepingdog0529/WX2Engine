#include "file_dialog.h"
#include <Windows.h>
#include <commdlg.h>
#include "debug/assert.h"


namespace wx2
{
	std::filesystem::path OpenFileDialog(const std::string& filter)
	{
		char fileName[MAX_PATH]{};
		char currentDir[MAX_PATH]{};

		OPENFILENAME ofn{};
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = sizeof(fileName);
		if (GetCurrentDirectory(MAX_PATH, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter.c_str();
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == FALSE)
			return "";

		return ofn.lpstrFile;
	}

	std::filesystem::path SaveFileDialog(const std::string& filter)
	{
		char fileName[MAX_PATH]{};
		char currentDir[MAX_PATH]{};

		OPENFILENAME ofn{};
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = sizeof(fileName);
		if (GetCurrentDirectory(MAX_PATH, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter.c_str();
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == FALSE)
			return "";

		return ofn.lpstrFile;
	}
}
