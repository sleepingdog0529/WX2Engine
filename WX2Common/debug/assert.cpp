#include "assert.h"
#include <format>
#include "log.h"

#if !defined(NDEBUG)

// BOOST_ASSERTéûÇÃìÆçÏÇíËã`
namespace boost
{
	void assertion_failed(const char* expr, const char* function, const char* file, long line)
	{
		auto out = std::format(
			"==============< Assertion failed ! >==============\n"
			"Expression: {}\n"
			"Function: {}\n"
			"File: {}\n"
			"Line: {}\n"
			"==================================================\n",
			expr, function, file, line);

		OutputDebugString(out.c_str());
		WX2_LOG_ERROR("\n{}", out);
		__debugbreak();
	}

	void assertion_failed_msg(char const* expr, char const* msg, char const* function, char const* file, long line)
	{
		auto out = std::format(
			"==============< Assertion failed ! >==============\n"
			"Expression: {}\n"
			"Message: {}\n"
			"Function: {}\n"
			"File: {}\n"
			"Line: {}\n"
			"==================================================\n",
			expr, msg, function, file, line);

		OutputDebugString(out.c_str());
		WX2_LOG_ERROR("\n{}", out);
		__debugbreak();
	}
}

#endif