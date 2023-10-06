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
		std::vector<VertexLayoutElement> Elements;
		uint32 Stride;

		// for update buffer data
		std::vector<uint32> Offsets;

	public:
		VertexBufferLayout()
			: Stride(0) {}

		template<typename T>
		void Push(uint32 count);

		template<>
		void Push<float>(uint32 count)
		{
			Elements.push_back({ GL_FLOAT, sizeof(GLfloat), count, GL_FALSE });
			Offsets.push_back(Stride);
			Stride += sizeof(GLfloat) * count;
		}

		template<>
		void Push<uint32>(uint32 count)
		{
			Elements.push_back({ GL_UNSIGNED_INT, sizeof(GLuint), count, GL_FALSE });
			Offsets.push_back(Stride);
			Stride += sizeof(GLuint) * count;
		}

		template<>
		void Push<unsigned char>(uint32 count)
		{
			Elements.push_back({ GL_UNSIGNED_BYTE, sizeof(GLubyte), count, GL_TRUE });
			Offsets.push_back(Stride);
			Stride += sizeof(GLubyte) * count;
		}

		inline const std::vector<VertexLayoutElement>& GetElements() const { return Elements; }

		inline uint32 GetStride() const { return Stride; }

		inline std::vector<uint32> GetOffsets() const { return Offsets; }
	};

	using VertexBufferLayout_SPtr = std::shared_ptr<VertexBufferLayout>;

}
