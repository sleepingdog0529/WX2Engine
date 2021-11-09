/*********************************************************************
 * @file   hash.h
 * @author Tomomi Murakami
 * @date   2021/11/09 23:57
 * @brief  �n�b�V���֐��I�u�W�F�N�g
 ********************************************************************/
#pragma once

namespace wx2
{
	/**
	 * @brief ������̃n�b�V���l�����߂�֐��I�u�W�F�N�g
	 */
	struct StringHash {
		using is_transparent = void;

		/**
		 * @brief     ������̃n�b�V���l�����߂�
		 * @param[in] ������r���[
		 * @return    �n�b�V���l
		 */
		std::size_t operator()(std::string_view strv) const {
			std::hash<std::string_view> hasher;
			return hasher(strv);
		}
	};
}