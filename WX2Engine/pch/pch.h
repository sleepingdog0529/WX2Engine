/*********************************************************************
 * @file	pch.h
 * @author	Tomomi Murakami
 * @date	2021/09/18 19:30
 * @brief	プリコンパイル済みヘッダ
 ********************************************************************/
#pragma once

#pragma warning(push, 0)

#include <WX2Common.h>
#include <WX2Input.h>
#include <WX2Graphics.h>
#include <WX2Mathematics.h>
#include <WX2Physics.h>
#include <WX2Model.h>

#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>

#include <bit>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <memory>
#include <mutex>
#include <numbers>
#include <numeric>
#include <optional>
#include <queue>
#include <ranges>
#include <sstream>
#include <thread>
#include <unordered_map>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>

#pragma warning(pop)