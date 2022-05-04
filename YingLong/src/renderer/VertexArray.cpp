
#include "YingLongPCH.h"

#include "VertexArray.h"
#include "RendererUtils.h"

namespace YingLong {

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void VertexArray::Init()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		uint64 offset = 0;
		for (uint32 i = 0; i < elements.size(); i++)
		{
			const auto& elem = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, elem.count, elem.type, elem.normalized,
				layout.GetStride(), (const void*)offset));
			offset += elem.count * elem.typeSize;
		}
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray::UnBind() const
	{
		GLCall(glBindVertexArray(0));
	}

}
