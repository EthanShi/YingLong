#pragma once

#include "YingLongPCH.h"

#include "RendererUtils.h"

namespace YingLong {

	struct YINGLONG_API VertexLayoutElement
	{
		uint32 type;
		uint32 typeSize;
		uint32 count;
		unsigned char normalized;
	};

	class YINGLONG_API VertexBufferLayout
	{
	private:
		std::vector<VertexLayoutElement> m_Elements;
		uint32 m_Stride;

		// for update buffer data
		std::vector<uint32> m_Offsets;

	public:
		VertexBufferLayout()
			: m_Stride(0) {}

		template<typename T>
		void Push(uint32 count);

		template<>
		void Push<float>(uint32 count)
		{
			m_Elements.push_back({ GL_FLOAT, sizeof(GLfloat), count, GL_FALSE });
			m_Offsets.push_back(m_Stride);
			m_Stride += sizeof(GLfloat) * count;
		}

		template<>
		void Push<uint32>(uint32 count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, sizeof(GLuint), count, GL_FALSE });
			m_Offsets.push_back(m_Stride);
			m_Stride += sizeof(GLuint) * count;
		}

		template<>
		void Push<unsigned char>(uint32 count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, sizeof(GLubyte), count, GL_TRUE });
			m_Offsets.push_back(m_Stride);
			m_Stride += sizeof(GLubyte) * count;
		}

		inline const std::vector<VertexLayoutElement>& GetElements() const { return m_Elements; }

		inline uint32 GetStride() const { return m_Stride; }

		inline std::vector<uint32> GetOffsets() const { return m_Offsets; }
	};

	using VertexBufferLayout_SPtr = std::shared_ptr<VertexBufferLayout>;

}
