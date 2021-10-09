/*********************************************************************
 * @file	string_helper.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 2:57
 * @brief	������^�̃w���p�[
*********************************************************************/
#pragma once

namespace wx2
{
	/// <summary>
	/// string_view��string�ɕϊ�
	/// </summary>
	/// <param name="v">������r���[</param>
	/// <returns>������</returns>
	inline std::string to_string(std::string_view v) {
		return { v.data(), v.size() };
	}
}