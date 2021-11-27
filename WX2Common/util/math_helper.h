/*********************************************************************
 * @file   math_helper.h
 * @author Tomomi Murakami
 * @date   2021/10/13 0:59
 * @brief  数学補助
 ********************************************************************/
#pragma once

namespace wx2
{
	/**
	 * @brief  正の値なら-1, 負の値なら-1, 0なら0を返す
	 * @param  val 判定する値
	 * @return 符号の値
	 */
	template <typename T>
	constexpr T Sign(T val) noexcept
	{
		return (val > T(0)) - (val < T(0));
	}

	/**
	 * @brief  ある範囲内にある値を別の範囲に当てはめ変換する
	 * @param  val 変換する値
	 * @param  inMin 最小入力範囲
	 * @param  inMax 最大入力範囲
	 * @param  outMin 最小出力範囲
	 * @param  outMax 最大出力範囲
	 * @return 変換した値
	 */
	template <typename T>
	constexpr T Remap(T val, T inMin, T inMax, T outMin, T outMax) noexcept
	{
		return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}
}