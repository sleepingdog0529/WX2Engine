/*********************************************************************
 * @file	window_property.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:19
 * @brief	ウィンドウ情報
 ********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#pragma warning(pop)

namespace wx2 
{
	/**
	 * @brief ウィンドウ情報
	 */
	struct WindowProperty
	{
	public:
		std::string title;
		int x, y;
		int width, height;
		unsigned long style;
		unsigned long ex_style;
		bool maximized;
		bool fullscreen;

	private:
		friend cereal::access;

		template <class Archive>
		void serialize(Archive& archive) noexcept
		{
			archive(
				CEREAL_NVP(title),
				CEREAL_NVP(x),
				CEREAL_NVP(y),
				CEREAL_NVP(width),
				CEREAL_NVP(height),
				CEREAL_NVP(style),
				CEREAL_NVP(ex_style),
				CEREAL_NVP(maximized),
				CEREAL_NVP(fullscreen));
		}
	};
}