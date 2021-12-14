/*********************************************************************
 * @file	assert.h
 * @author	Tomomi Murakami
 * @date	2021/09/18 23:45
 * @brief	アサーション
 ********************************************************************/
#pragma once
#pragma warning(push, 0)
#define BOOST_ENABLE_ASSERT_HANDLER // boostのassertの処理内容を書き換えるためのフラグ
#include <boost/assert.hpp>
#pragma warning(pop)

#if defined(NDEBUG)

#define WX2_ASSERT(expr) ((void)0)
#define WX2_ASSERT_MSG(expr, msg) ((void)0)

#elif !defined(NDEBUG)

/**
 * @brief デバッグビルド時にログを出力してブレークポイントを発生させる
 * @param expr 判定式
 */
#define WX2_ASSERT(expr) BOOST_ASSERT(expr)

/**
 * @brief デバッグビルド時にメッセージ付きでログを出力してブレークポイントを発生させる
 * @param expr 判定式
 * @param msg 書式文字列
 * @param __VA_ARGS__ 書式引数
 */
#define WX2_ASSERT_MSG(expr, msg, ...) BOOST_ASSERT_MSG(expr, std::format(msg, ##__VA_ARGS__).c_str())

#endif