#pragma once

#include "core/Macros.h"
#include "core/Log.h"

DECLARE_LOGGER(RendererUtilsLog)

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int32 line);


#define VIEWPORT_UNIFORM_NAME "viewport"
#define UNIT_SCALE_UNIFORM_NAME "UnitScale"

#define DEFAULT_WINDOW_HEIGHT 1080
#define DEFAULT_WINDOW_WIDTH 1920