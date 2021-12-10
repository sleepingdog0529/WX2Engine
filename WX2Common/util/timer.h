/*********************************************************************
 * @file   timer.h
 * @author Tomomi Murakami
 * @date   2021/11/28 10:17
 * @brief  �^�C�}�[
*********************************************************************/
#pragma once
#include <chrono>
#include "macro.h"

namespace wx2
{
	using Frame = std::chrono::duration<int32_t, std::ratio<1, 120>>;

	template <class Units = std::chrono::milliseconds >
	class Timer final
	{
	public:
		Timer() noexcept
			: start_(std::chrono::system_clock::now())
		{
		}

		~Timer() = default;

		WX2_DISALLOW_COPY_AND_MOVE(Timer);

		void Start() noexcept
		{
			start_ = std::chrono::system_clock::now();
		}

		Units ElapcedTime() const noexcept
		{
			return std::chrono::duration_cast<Units>(
				std::chrono::system_clock::now() - start_);
		}

	private:
		std::chrono::time_point<std::chrono::system_clock> start_{};
	};
}
