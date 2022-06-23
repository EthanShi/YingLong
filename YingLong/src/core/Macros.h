#pragma once

#include <stddef.h>
#include <glm/glm/glm.hpp>

#ifndef _WIN64
	#error YingLong only surport windows x64 platform!
#endif

#ifdef YINGLONG_BUILD_DLL
	#ifdef YINGLONG_BUILD_LIB
		#define YINGLONG_API __declspec( dllexport )
	#else
		#define YINGLONG_API __declspec( dllimport )
	#endif
#else
	#define YINGLONG_API
#endif

#define ASSERT(x) if(!(x)) __debugbreak();

// type alias
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using wchar = wchar_t;
using char16 = char16_t;
using char32 = char32_t;


// precisions

#define DOUBLE_PERCISION 0.00001f
#define DOUBLE_EQUAL(x, y) (glm::abs((x) - (y)) < DOUBLE_PERCISION)