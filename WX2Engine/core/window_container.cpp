#include "window_container.h"
#include "constant.h"

namespace wx2 
{
	WindowContainer::WindowContainer()
	{
		Deserialize();
	}

	WindowContainer::~WindowContainer()
	{
		windowProps_.clear();
		std::ranges::for_each(windows_,
			[this](const std::pair<std::string, WindowPtr>& p)
		{
			windowProps_.try_emplace(p.first, p.second->GetWindowProperty());
		});

		Serialize();
	}

	WindowContainer::WindowPtr WindowContainer::Create(const std::string& name, const WindowProperty& defaultProp)
	{
		auto [propItr, unuse] = windowProps_.try_emplace(name, defaultProp);

		auto [wndItr, success] = windows_.try_emplace(name, std::make_shared<Window>(this, propItr->second));
		if (!success)
		{
			WX2_LOG_CRITICAL("ウィンドウ名が重複しています。ウィンドウ名: {}", name);
			exit(EXIT_FAILURE);
		}

		return wndItr->second;
	}

	LRESULT WindowContainer::WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) const
	{
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	void WindowContainer::Serialize()
	{
		auto filePath = std::filesystem::path(CONFIG_DIR) / WINDOW_PROPERTY_FILE_NAME_;
		
		std::ofstream ofs(filePath, std::ios::out);
		cereal::JSONOutputArchive output(ofs);
		output(cereal::make_nvp("windowContainer", windowProps_));
	}

	void WindowContainer::Deserialize()
	{
		auto filePath = std::filesystem::path(CONFIG_DIR) / WINDOW_PROPERTY_FILE_NAME_;

		if (std::filesystem::exists(filePath))
		{
			std::ifstream ifs(filePath, std::ios::in);
			cereal::JSONInputArchive input(ifs);
			input(cereal::make_nvp("windowContainer", windowProps_));
		}
	}
}