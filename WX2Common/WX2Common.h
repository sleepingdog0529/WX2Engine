/*********************************************************************
 * @file	WX2Common.h
 * @author	Tomomi Murakami
 * @date	2021/11/02 10:28
 * @brief	‹¤’Êƒwƒbƒ_
 ********************************************************************/
#pragma once

#pragma warning(push, 0)
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#pragma warning(pop)

#include "core/window_property.h"
#include "core/layer_stack.h"
#include "debug/assert.h"
#include "debug/log.h"
#include "debug/com_exception.h"
#include "debug/runtime_errer.h"
#include "debug/debugging.h"
#include "util/hash.h"
#include "util/macro.h"
#include "util/event_dispatcher.h"
#include "util/safe_release.h"
#include "util/string_helper.h"
#include "util/thread_pool.h"
#include "util/timer.h"