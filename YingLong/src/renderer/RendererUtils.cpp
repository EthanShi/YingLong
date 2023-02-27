
#include "YingLongPCH.h"

#include "RendererUtils.h"

DEFINE_LOGGER(RendererUtilsLog)

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int32 line)
{
	GLenum error = glGetError();
	while (error)
	{
		RendererUtilsLog().error("[OpenGL Error] ({}): {} : {} : {}", error, function, file, line);
		return false;
	}
	return true;
}