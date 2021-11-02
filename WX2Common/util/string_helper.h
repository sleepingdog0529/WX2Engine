/*********************************************************************
 * @file	string_helper.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 2:57
 * @brief	������^�̃w���p�[
*********************************************************************/
#pragma once

namespace wx2
{
	/**
	 * @brief	  string_view��string�ɕϊ�����
	 * @param[in] strv ������r���[
	 * @return    ������
	 */
	inline std::string ToString(std::string_view strv) 
	{
		return { strv.data(), strv.size() };
	}
}