/*********************************************************************
 * @file   timer.h
 * @author Tomomi Murakami
 * @date   2021/11/28 10:17
 * @brief  タイマー
*********************************************************************/
#pragma once
#include <chrono>
#include "macro.h"

namespace wx2
{
	template <class Units = std::chrono::milliseconds >
	class Timer final
	{
	public:
		Timer() noexcept
			: start_(std::chrono::steady_clock::now())
		{
		}

		~Timer() = default;

		WX2_DISALLOW_COPY_AND_MOVE(Timer);

		void Start() noexcept
		{
			start_ = std::chrono::steady_clock::now();
		}

		Units ElapcedTime() const noexcept
		{
			return std::chrono::duration_cast<Units>(
				std::chrono::steady_clock::now() - start_);
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> start_{};
	};
}
