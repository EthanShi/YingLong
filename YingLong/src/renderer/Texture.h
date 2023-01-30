#pragma once

#include <string>

#include "core/Macros.h"
#include "core/Log.h"

DECLARE_LOGGER(TextureLog)

namespace YingLong {

	class YINGLONG_API Texture
	{
		friend class TextureManager;

	public:
		Texture() = default;
		~Texture();

		bool IsValid() const;

		void Bind(uint32 slot) const;
		void UnBind() const;

		inline uint32 GetTexutureID() { return m_TextureID; }
		inline int32 GetWidth() const { return m_Width; }
		inline int32 GetHeight() const { return m_Height; }

	private:
		void Load(const std::string& filepath);

	private:
		uint32 m_TextureID = 0;
		uint32 m_RendererID = 0;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer = nullptr;
		int32 m_Width = 0;
		int32 m_Height = 0;
		int32 m_BPP = 0;
	};

	using Texture_SPtr = std::shared_ptr<Texture>;

	class YINGLONG_API TextureManager
	{
	public:
		uint32 LoadTexture(const std::string& FileName);
		Texture& GetTexture(uint32 TextureID);

	private:
		uint32 m_TextureID = 0;
		Texture m_InvalidTexture;
		std::unordered_map<std::string, Texture> m_LoadedTextureMap;
		std::unordered_map<uint32, std::string> m_LoadedTextureMapPath;
	};
}

