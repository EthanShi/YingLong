
#include "YingLongPCH.h"

#include "IndexBuffer.h"
#include "RendererUtils.h"

namespace YingLong {

	IndexBuffer::IndexBuffer(const uint32* data, uint32 count)
	{
		Init(data, count);
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &RendererID));
	}

	void IndexBuffer::Init(const uint32* data, uint32 count)
	{
		ASSERT(sizeof(uint32) == sizeof(GLuint))

		Count = count;

		GLCall(glGenBuffers(1, &RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32), data, GL_STATIC_DRAW));
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID));
	}

	void IndexBuffer::UnBind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

}