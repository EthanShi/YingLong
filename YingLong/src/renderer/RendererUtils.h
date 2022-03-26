#pragma once

#include "core/Macros.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int32 line);


#define VIEWPORT_UNIFORM_NAME "viewport"
#define MVP_UNIFORM_NAME "MVP"

#define DEFAULT_WINDOW_HEIGHT 1080
#define DEFAULT_WINDOW_WIDTH 1920