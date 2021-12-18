/*********************************************************************
 * @file	macro.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:37
 * @brief	マクロ宣言
 ********************************************************************/
#pragma once

/**
 * @brief デフォルトコピーコンストラクタと代入演算子を定義する
 * @param name クラス名
 */
#define WX2_COPYABLE(name)					\
	name(const name&) = default;			\
	name& operator = (const name&) = default

 /**
  * @brief デフォルトムーブコンストラクタと移動代入演算子を定義する
  * @param name クラス名
  */
#define WX2_MOVEABLE(name)					\
	name(name&&) = default;					\
	name& operator = (name&&) = default

/**
 * @brief コピーコンストラクタと代入演算子の呼び出しを禁止する
 * @param name クラス名
 */
#define WX2_DISALLOW_COPY(name)				\
	name(const name&) = delete;				\
	name& operator = (const name&) = delete

/**
 * @brief ムーブコンストラクタと移動代入演算子の呼び出しを禁止する
 * @param name クラス名
 */
#define WX2_DISALLOW_MOVE(name)				\
	name(name&&) = delete;					\
	name& operator = (name&&) = delete