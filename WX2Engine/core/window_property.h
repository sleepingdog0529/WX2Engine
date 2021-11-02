/*********************************************************************
 * @file	window_property.h
 * @author	Tomomi Murakami
 * @date	2021/09/20 19:19
 * @brief	ウィンドウ情報
 ********************************************************************/
#pragma once
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>

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
		bool fullscreen;

	private:
		friend cereal::access;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(
				CEREAL_NVP(title),
				CEREAL_NVP(x),
				CEREAL_NVP(y),
				CEREAL_NVP(width),
				CEREAL_NVP(height),
				CEREAL_NVP(fullscreen));
		}
	};
}