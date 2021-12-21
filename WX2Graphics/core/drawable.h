/*********************************************************************
 * @file   drawable.h
 * @author Tomomi Murakami
 * @date   2021/12/20 12:12
 * @brief  描画可能なオブジェクト
 ********************************************************************/
#pragma once
#include <WX2Common.h>
#include "device.h"

namespace wx2
{
	class Drawable
	{
	public:
		Drawable() = default;
		virtual ~Drawable() = default;

		WX2_COPYABLE(Drawable);
		WX2_MOVEABLE(Drawable);

		virtual void Draw() const noexcept = 0;

	protected:
		void Initialize(Device* devices) noexcept
		{
			WX2_ASSERT_MSG(devices, "デバイスがnullptrでした。");
			devices_ = devices;
		}

		Device* devices_{};
	};
}
