/*********************************************************************
 * @file	pch.h
 * @author	Tomomi Murakami
 * @date	2021/09/18 19:30
 * @brief	プリコンパイル済みヘッダ
*********************************************************************/
#pragma once

#include <WX2Common.h>
#include <WX2Input.h>

#pragma warning(push, 0) 
#include <memory>
#include <filesystem>
#include <chrono>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <fstream>
#include <ranges>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <memory>
#include <iostream>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>

#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#pragma warning(pop)