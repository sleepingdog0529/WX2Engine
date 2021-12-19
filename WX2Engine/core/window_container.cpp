#include "window_container.h"
#include "constant.h"

namespace wx2
{
	WindowContainer::WindowContainer() noexcept
	{
		Deserialize();
	}

	WindowContainer::~WindowContainer() noexcept
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
		WX2_RUNTIME_ERROR_IF_FAILED(success, "ウィンドウ名が重複しています。ウィンドウ名: {}", name);

		return wndItr->second;
	}

	WindowContainer::EventListenerType WindowContainer::AppendCallback(
		const WindowEvent event, 
		const EventCallbackType& callback) noexcept
	{
		auto listener = EventListenerType(callback);
		eventDispatcher_.AppendListener(event, listener);
		return listener;
	}

	bool WindowContainer::ProcessMessage() noexcept
	{
		MSG msg = {};

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
			{
				return false;
			}
		}

		return true;
	}

	LRESULT WindowContainer::WindowProcedure(const HWND hwnd, const UINT msg, const WPARAM wp, const LPARAM lp) noexcept
	{
		eventDispatcher_.Dispatch(static_cast<WindowEvent>(msg), hwnd, wp, lp);

		return DefWindowProc(hwnd, msg, wp, lp);
	}

	void WindowContainer::Serialize() noexcept
	{
		const auto filePath = std::filesystem::path(CONFIG_DIR) / PROPERTY_FILE_NAME;

		std::ofstream ofs(filePath, std::ios::out);
		cereal::JSONOutputArchive output(ofs);
		output(cereal::make_nvp("windowContainer", windowProps_));
	}

	void WindowContainer::Deserialize() noexcept
	{
		if (const auto filePath = std::filesystem::path(CONFIG_DIR) / PROPERTY_FILE_NAME; std::filesystem::exists(filePath))
		{
			std::ifstream ifs(filePath, std::ios::in);
			cereal::JSONInputArchive input(ifs);
			input(cereal::make_nvp("windowContainer", windowProps_));
		}
	}
}