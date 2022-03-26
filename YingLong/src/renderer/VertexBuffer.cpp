

#include "YingLongPCH.h"

#include "VertexBuffer.h"
#include "RendererUtils.h"

namespace YingLong {

	VertexBuffer::VertexBuffer(const void* data, uint32 size, int32 usage)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GLenum(usage)));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void VertexBuffer::UnBind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::UpdateData(uint32 dataIndex, const VertexBufferLayout& layout, uint32 elementIndex, const void* data)
	{
		Bind();
		const VertexLayoutElement& layoutElement = layout.GetElements()[elementIndex];
		GLCall(glBufferSubData(GL_ARRAY_BUFFER,
			dataIndex * layout.GetStride() + layout.GetOffsets()[elementIndex],
			layoutElement.count * layoutElement.typeSize,
			data));
	}

}


