/*********************************************************************
 * @file	leak_detector.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:40
 * @brief	���������[�N���o
 ********************************************************************/
#pragma once

// �f�o�b�O���Ȃ烁�������[�N�����o����
#if !define(NDEBUG)
#pragma warning(push, 0)
#include <vld/vld.h>
#pragma warning(pop)
#endif