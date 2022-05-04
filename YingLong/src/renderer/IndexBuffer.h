#pragma once

#include "core/Macros.h"

namespace YingLong {

	class YINGLONG_API IndexBuffer
	{
	private:
		uint32 m_RendererID;
		uint32 m_Count;

	public:
		IndexBuffer() = default;
		IndexBuffer(const uint32* data, uint32 count);
		~IndexBuffer();

		void Init(const uint32* data, uint32 count);

		void Bind() const;
		void UnBind() const;

		inline uint32 GetCount() const { return m_Count; }
	};

	using IndexBuffer_SPtr = std::shared_ptr<IndexBuffer>;

}
