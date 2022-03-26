#pragma once

#include "core/Macros.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace YingLong {

	class YINGLONG_API VertexArray
	{
	private:
		uint32 m_RendererID;

	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void UnBind() const;
	};

}
