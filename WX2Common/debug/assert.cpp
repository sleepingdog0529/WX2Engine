#include "assert.h"
#include <format>
#include "log.h"

#if !defined(NDEBUG)

// BOOST_ASSERT時の動作を定義
namespace boost
{
	void assertion_failed(
		const char* expr,
		const char* function,
		const char* file, 
		const long line)
	{
		auto out = std::format(
			"\n"
			"==============< Assertion failed ! >==============\n"
			"Expression: {}\n"
			"File: {}\n"
			"Function: {}\n"
			"Line: {}\n"
			"==================================================\n",
			expr, file, function, line);

		OutputDebugString(out.c_str());
		WX2_LOG_ERROR("\n{}", out);
		__debugbreak();
	}

	void assertion_failed_msg(
		char const* expr, 
		char const* msg, 
		char const* function, 
		char const* file, 
		const long line)
	{
		auto out = std::format(
			"\n"
			"==============< Assertion failed ! >==============\n"
			"Expression: {}\n"
			"Message: {}\n"
			"File: {}\n"
			"Function: {}\n"
			"Line: {}\n"
			"==================================================\n",
			expr, msg, file, function, line);

		OutputDebugString(out.c_str());
		WX2_LOG_ERROR("\n{}", out);
		__debugbreak();
	}
}

#endif