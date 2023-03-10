#pragma once

#include "PlatformDetection.h"

#include <memory>

#ifdef GE_DEBUG
	#ifdef GE_PLATFORM_WINDOWS
		#define GE_DEBUGBREAK() __debugbreak()
	#else
		#error "Platform doesnt support debugbreak yet!"
	#endif
		#define GE_ENABLE_ASSERTS
#else
	#define GE_DEBUGBREAK()
#endif

#define GE_STRINGIFY_MACRO(x) #x
#define GE_EXPAND_MACRO(x) x

#define BIND_FUNCTION(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...);}


namespace GE
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	};

	template<typename T, typename ... Args>
	constexpr Scope<T> MakeScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	};

	template<typename To, typename From>
	To* Cast(From* from)
	{
		return dynamic_cast<To*>(from);
	};

	template<typename To, typename From>
	Ref<To> Cast_Ptr(const Ref<From>& from)
	{
		return std::dynamic_pointer_cast<To>(from);
	};
}

#include "Log.h"
#include "Assert.h"