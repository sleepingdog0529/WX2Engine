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
		//! アッパーケースのRelease関数を持っているか判定する
		template <class T>
		concept HasReleaseUpperCase = requires (T x)
		{
			x.Release();
		};

		//! ローワーケースのrelease関数を持っているか判定する
		template <class T>
		concept HasReleaseLowerCase = requires (T x)
		{
			x.release();
		};
	}

	/**
	 * @brief  nullチェックを行ったうえでアッパーケースのRelease関数を呼び出し解放する
	 * @tparam T 解放対象の型
	 * @param  x 解放対象
	 */
	template <detail::HasReleaseUpperCase T>
	inline void SafeRelease(T*& x) noexcept
	{
		if (x)
		{
			x->Release();
			x = nullptr;
		}
	}

	/**
	 * @brief  nullチェックを行ったうえでローワーケースのrelease関数を呼び出し解放する
	 * @tparam T 解放対象の型
	 * @param  x 解放対象
	 */
	template <detail::HasReleaseLowerCase T>
	inline void SafeRelease(T*& x) noexcept
	{
		if (x)
		{
			x->release();
			x = nullptr;
		}
	}

	template<typename T = void>
	struct SafeDeleter
	{
		void operator()(T* x) const
		{
			SafeRelease(x);
		}
	};
}
