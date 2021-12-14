/*********************************************************************
 * @file	leak_detector.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:40
 * @brief	メモリリーク検出
 ********************************************************************/
#pragma once

// デバッグ中ならメモリリークを検出する
#if !define(NDEBUG)
#pragma warning(push, 0)
#include <vld/vld.h>
#pragma warning(pop)
#endif