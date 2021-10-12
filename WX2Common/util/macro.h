/*********************************************************************
 * @file	macro.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:37
 * @brief	マクロ宣言
*********************************************************************/
#pragma once

/// <summary>
/// 1 を n 回左ビットシフトした値を返す
/// </summary>
/// <param name="n">左ビットシフト回数</param>
/// <returns>1 を指定回数左ビットシフトした値</returns>
#define WX2_BIT(n) (1 << (n))

/// <summary>
/// コピーコンストラクタの呼び出しを禁止する
/// </summary>
#define WX2_DISALLOW_COPY(name)				\
	name(const name&) = delete;				\
	name& operator=(const name&) = delete

/// <summary>
/// ムーブコンストラクタの呼び出しを禁止する
/// </summary>
#define WX2_DISALLOW_MOVE(name)				\
	name(name&&) = delete;					\
	name& operator=(name&&) = delete

/// <summary>
/// ムーブ＆コピーコンストラクタの呼び出しを禁止する
/// </summary>
#define WX2_DISALLOW_COPY_AND_MOVE(name)	\
	WX2_DISALLOW_COPY(name);				\
	WX2_DISALLOW_MOVE(name)