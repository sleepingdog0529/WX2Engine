/*********************************************************************
 * @file   debugging.h
 * @author Tomomi Murakami
 * @date   2021/11/22 23:38
 * @brief  
 ********************************************************************/
#pragma once

namespace wx2
{
	//! �f�o�b�O�r���h����Ă��邩
#if defined(NDEBUG)
	static constexpr bool IS_DEBUGGING = false;
#else
	static constexpr bool IS_DEBUGGING = true;
#endif
}