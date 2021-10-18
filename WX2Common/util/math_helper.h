/*********************************************************************
 * @file	math_helper.h
 * @author	Tomomi Murakami
 * @date	2021/10/13 0:59
 * @brief	数学補助
*********************************************************************/
#pragma once

namespace wx2
{
	/// <summary>
	/// 正の値なら-1, 負の値なら-1, 0なら0を返す
	/// </summary>
	/// <param name="val">判定する値</param>
	/// <returns>符号値</returns>
	template <typename T>
	constexpr T Sign(T val)
	{
		return (val > T(0)) - (val < T(0));
	}

	template <typename T>
	constexpr T Remap(T val, T inMin, T inMax, T outMin, T outMax)
	{
		return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}
}