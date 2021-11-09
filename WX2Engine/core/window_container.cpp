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
		std::for_each(windows_.begin(), windows_.end(),
			[this](const std::pair<std::string, WindowPtr>& p)
		{
			windowProps_.emplace(p.first, p.second->GetWindowProperty());
		});

		Serialize();
	}

	WindowContainer::WindowPtr WindowContainer::Create(const std::string& name, const WindowProperty& defaultProp)
	{
		auto [propItr, unuse] = windowProps_.try_emplace(name, defaultProp);

		auto [wndItr, success] = windows_.emplace(name, std::make_shared<Window>(this, propItr->second));
		if (!success)
		{
			WX2_LOG_CRITICAL("ウィンドウ名が重複しています。ウィンドウ名: {}", name);
			exit(EXIT_FAILURE);
		}

		return wndItr->second;
	}

	void WindowContainer::ProcessMessages(std::function<bool()> process)
	{
		MSG msg = {};

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if (!process()) break;
			}
		}
	}

	LRESULT WindowContainer::WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	void WindowContainer::Serialize()
	{
		auto filePath = std::filesystem::path(CONFIG_DIR) / WINDOW_PROPERTY_FILE_NAME_;
		
		std::stringstream os;
		{
			cereal::JSONOutputArchive output(os);
			output(cereal::make_nvp("windowContainer", windowProps_));
		}
		std::ofstream ofs(filePath, std::ios::out);
		ofs << os.str() << std::endl;
	}

	void WindowContainer::Deserialize()
	{
		auto filePath = std::filesystem::path(CONFIG_DIR) / WINDOW_PROPERTY_FILE_NAME_;

		if (std::filesystem::exists(filePath))
		{
			std::stringstream is;
			std::ifstream ifs(filePath, std::ios::in);
			is << ifs.rdbuf() << std::endl;
			cereal::JSONInputArchive input(is);
			input(cereal::make_nvp("windowContainer", windowProps_));
		}
	}
}