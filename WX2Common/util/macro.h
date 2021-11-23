/*********************************************************************
 * @file	macro.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:37
 * @brief	�}�N���錾
 ********************************************************************/
#pragma once

/**
 * @brief �R�s�[�R���X�g���N�^�̌Ăяo�����֎~����
 * @param name �N���X��
 */
#define WX2_DISALLOW_COPY(name)				\
	name(const name&) = delete;				\
	name& operator=(const name&) = delete

/**
 * @brief ���[�u�R���X�g���N�^�̌Ăяo�����֎~����
 * @param name �N���X��
 */
#define WX2_DISALLOW_MOVE(name)				\
	name(name&&) = delete;					\
	name& operator=(name&&) = delete

 /**
  * @brief �R�s�[�����[�u�R���X�g���N�^�̌Ăяo�����֎~����
  * @param name �N���X��
  */
#define WX2_DISALLOW_COPY_AND_MOVE(name)	\
	WX2_DISALLOW_COPY(name);				\
	WX2_DISALLOW_MOVE(name)