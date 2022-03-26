#pragma once

#include "core/Macros.h"

namespace YingLong {

	class YINGLONG_API IndexBuffer
	{
	private:
		uint32 m_RendererID;
		uint32 m_Count;

	public:
		IndexBuffer(const uint32* data, uint32 count);
		~IndexBuffer();

		void Bind() const;
		void UnBind() const;

		inline uint32 GetCount() const { return m_Count; }
	};

}
