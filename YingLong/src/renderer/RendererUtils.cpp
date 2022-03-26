
#include "YingLongPCH.h"

#include "RendererUtils.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int32 line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << std::hex << "0x" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}