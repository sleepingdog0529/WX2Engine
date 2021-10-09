/*********************************************************************
 * @file	leak_detector.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:40
 * @brief	メモリリーク検出
*********************************************************************/
#pragma once

// メモリリークを調べる
#define WX2_LEAK_DETECTION 0

#if WX2_LEAK_DETECTION
#include <vld/vld.h>
#endif