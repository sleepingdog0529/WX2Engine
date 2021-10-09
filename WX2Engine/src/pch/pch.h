#pragma once

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

#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#pragma warning(pop)

#include "core/macro.h"
#include "core/log.h"
#include "core/assert.h"