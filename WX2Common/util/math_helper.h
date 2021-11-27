/*********************************************************************
 * @file   math_helper.h
 * @author Tomomi Murakami
 * @date   2021/10/13 0:59
 * @brief  ���w�⏕
 ********************************************************************/
#pragma once

namespace wx2
{
	/**
	 * @brief  ���̒l�Ȃ�-1, ���̒l�Ȃ�-1, 0�Ȃ�0��Ԃ�
	 * @param  val ���肷��l
	 * @return �����̒l
	 */
	template <typename T>
	constexpr T Sign(T val) noexcept
	{
		return (val > T(0)) - (val < T(0));
	}

	/**
	 * @brief  ����͈͓��ɂ���l��ʂ͈̔͂ɓ��Ă͂ߕϊ�����
	 * @param  val �ϊ�����l
	 * @param  inMin �ŏ����͔͈�
	 * @param  inMax �ő���͔͈�
	 * @param  outMin �ŏ��o�͔͈�
	 * @param  outMax �ő�o�͔͈�
	 * @return �ϊ������l
	 */
	template <typename T>
	constexpr T Remap(T val, T inMin, T inMax, T outMin, T outMax) noexcept
	{
		return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}
}