/*********************************************************************
 * @file   safe_release.h
 * @author Tomomi Murakami
 * @date   2021/11/29 15:34
 * @brief  nullチェックの上解放
 ********************************************************************/
#pragma once

namespace wx2
{
	namespace detail
	{
		template <class T>
		concept HasReleaseUpperCase = requires (T x)
		{
			x.Release();
		};

		template <class T>
		concept HasReleaseLowerCase = requires (T x)
		{
			x.release();
		};
	}

	template <detail::HasReleaseUpperCase T>
	inline void SafeRelease(T*& x) noexcept
	{
		if (x)
		{
			x->Release();
			x = nullptr;
		}
	}

	template <detail::HasReleaseLowerCase T>
	inline void SafeRelease(T*& x) noexcept
	{
		if (x)
		{
			x->release();
			x = nullptr;
		}
	}
}
