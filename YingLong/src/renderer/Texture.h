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

		inline uint32 GetTexutureID() { return TextureID; }
		inline int32 GetWidth() const { return Width; }
		inline int32 GetHeight() const { return Height; }

	private:
		void Load(const std::string& filepath);

	private:
		uint32 TextureID = 0;
		uint32 RendererID = 0;
		std::string FilePath;
		unsigned char* LocalBuffer = nullptr;
		int32 Width = 0;
		int32 Height = 0;
		int32 BPP = 0;
	};

	using Texture_SPtr = std::shared_ptr<Texture>;

	class YINGLONG_API TextureManager
	{
	public:
		uint32 LoadTexture(const std::string& FileName);
		Texture& GetTexture(uint32 TextureID);

	private:
		uint32 TextureID = 0;
		Texture InvalidTexture;
		std::unordered_map<std::string, Texture> LoadedTextureMap;
		std::unordered_map<uint32, std::string> LoadedTextureMapPath;
	};
}

