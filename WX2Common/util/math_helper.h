/*********************************************************************
 * @file	math_helper.h
 * @author	Tomomi Murakami
 * @date	2021/10/13 0:59
 * @brief	���w�⏕
*********************************************************************/
#pragma once

namespace wx2
{
	/// <summary>
	/// ���̒l�Ȃ�-1, ���̒l�Ȃ�-1, 0�Ȃ�0��Ԃ�
	/// </summary>
	/// <param name="val">���肷��l</param>
	/// <returns>�����l</returns>
	template <typename T>
	T Sign(T val)
	{
		return (val > T(0)) - (val < T(0));
	}
}