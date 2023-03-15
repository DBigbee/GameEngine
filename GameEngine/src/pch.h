#pragma once

#include "Core/PlatformDetection.h"

#ifdef GE_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <optional>
#include <set>
#include <limits>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <unordered_set>


#include "Core/Core.h"
#include "Core/Log.h"

#ifdef GE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif