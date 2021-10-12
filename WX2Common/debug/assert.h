/*********************************************************************
 * @file	assert.h
 * @author	Tomomi Murakami
 * @date	2021/09/18 23:45
 * @brief	�A�T�[�V����
*********************************************************************/
#pragma once
#pragma warning(push, 0) 
#define BOOST_ENABLE_ASSERT_HANDLER
#include <boost/assert.hpp>
#pragma warning(pop)

#if defined(NDEBUG)

#define WX2_ASSERT(expr) ((void)0)
#define WX2_ASSERT_MSG(expr, msg) ((void)0)

#elif !defined(NDEBUG)

/// <summary>
/// �f�o�b�O�r���h���Ƀ��O���o�͂��ău���[�N�|�C���g�𔭐�������
/// </summary>
/// <param name="expr">���莮</param>
#define WX2_ASSERT(expr) BOOST_ASSERT(expr);

/// <summary>
/// �f�o�b�O�r���h���Ƀ��b�Z�[�W�t���Ń��O���o�͂��ău���[�N�|�C���g�𔭐�������
/// </summary>
/// <param name="expr">���莮</param>
/// <param name="msg">����������</param>
/// <param name="__VA_ARGS__">��������</param>
#define WX2_ASSERT_MSG(expr, msg, ...) BOOST_ASSERT_MSG(expr, std::format(msg, ##__VA_ARGS__).c_str());

#endif