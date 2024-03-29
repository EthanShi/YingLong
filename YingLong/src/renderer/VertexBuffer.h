#pragma once

#include "core/Macros.h"

#include "VertexBufferLayout.h"

namespace YingLong {

	class YINGLONG_API VertexBuffer
	{
	private:
		uint32 RendererID;

	public:
		VertexBuffer() = default;
		VertexBuffer(const void* data, uint32 size, int32 usage);
		~VertexBuffer();

		void Init(const void* data, uint32 size, int32 usage);

		void Bind() const;
		void UnBind() const;

		void UpdateData(uint32 dataIndex, const VertexBufferLayout& layout, uint32 elementIndex, const void* data);
	};

	using VertexBuffer_SPtr = std::shared_ptr<VertexBuffer>;

}

