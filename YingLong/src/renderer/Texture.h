#pragma once

#include <string>

#include "core/Macros.h"

namespace YingLong {

	class YINGLONG_API Texture
	{
	private:
		uint32 m_RendererID;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int32 m_Width, m_Height, m_BPP;

	public:
		Texture(const std::string filepath);
		~Texture();

		void Bind(uint32 slot) const;
		void UnBind() const;

		inline int32 GetWidth() const { return m_Width; }
		inline int32 GetHeight() const { return m_Height; }
	};

}

