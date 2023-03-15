#pragma once

#include "Core.h"
#include "Log.h"
#include <filesystem>


#ifdef GE_ENABLE_ASSERTS
#define GE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
	{                                                  \
		if (!(check))                                  \
		{                                              \
			GE##type##ERROR(msg, __VA_ARGS__);         \
			GE_DEBUGBREAK();                           \
		}                                              \
	}
#define GE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) GE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define GE_INTERNAL_ASSERT_NO_MSG(type, check) GE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2} ", GE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define GE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define GE_INTERNAL_ASSERT_GET_MACRO(...) GE_EXPAND_MACRO(GE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, GE_INTERNAL_ASSERT_WITH_MSG, GE_INTERNAL_ASSERT_NO_MSG))

// optional message
#define GE_ASSERT(...) GE_EXPAND_MACRO(GE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define GE_CORE_ASSERT(...) GE_EXPAND_MACRO(GE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#define GE_ASSERT(...)
#define GE_CORE_ASSERT(...)
#endif