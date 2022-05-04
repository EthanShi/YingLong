#pragma once

#include <string>

#include "core/Macros.h"

namespace YingLong {

	class YINGLONG_API Texture
	{
	private:
		uint32 m_RendererID = 0;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer = nullptr;
		int32 m_Width = 0;
		int32 m_Height = 0;
		int32 m_BPP = 0;

	public:
		Texture() = default;
		Texture(const std::string& filepath);
		~Texture();

		void Load(const std::string& filepath);

		void Bind(uint32 slot) const;
		void UnBind() const;

		inline int32 GetWidth() const { return m_Width; }
		inline int32 GetHeight() const { return m_Height; }
	};

	using Texture_SPtr = std::shared_ptr<Texture>;

}

