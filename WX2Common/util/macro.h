/*********************************************************************
 * @file	macro.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:37
 * @brief	�}�N���錾
*********************************************************************/
#pragma once

/// <summary>
/// 1 �� n �񍶃r�b�g�V�t�g�����l��Ԃ�
/// </summary>
/// <param name="n">���r�b�g�V�t�g��</param>
/// <returns>1 ���w��񐔍��r�b�g�V�t�g�����l</returns>
#define WX2_BIT(n) (1 << (n))

/// <summary>
/// �R�s�[�R���X�g���N�^�̌Ăяo�����֎~����
/// </summary>
#define WX2_DISALLOW_COPY(name)				\
	name(const name&) = delete;				\
	name& operator=(const name&) = delete

/// <summary>
/// ���[�u�R���X�g���N�^�̌Ăяo�����֎~����
/// </summary>
#define WX2_DISALLOW_MOVE(name)				\
	name(name&&) = delete;					\
	name& operator=(name&&) = delete

/// <summary>
/// ���[�u���R�s�[�R���X�g���N�^�̌Ăяo�����֎~����
/// </summary>
#define WX2_DISALLOW_COPY_AND_MOVE(name)	\
	WX2_DISALLOW_COPY(name);				\
	WX2_DISALLOW_MOVE(name)