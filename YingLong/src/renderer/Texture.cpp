
#include "YingLongPCH.h"

#include "Texture.h"
#include "renderer/Renderer3D.h"

DEFINE_LOGGER(TextureLog)

namespace YingLong {

	Texture::~Texture()
	{
		GLCall(glDeleteTextures(1, &RendererID));
	}

	bool Texture::IsValid() const
	{
		return TextureID > 0;
	}

	void Texture::Load(const std::string& filepath)
	{
		FilePath = filepath;

		stbi_set_flip_vertically_on_load(1);
		LocalBuffer = stbi_load(filepath.c_str(), &Width, &Height, &BPP, 3);

		GLCall(glGenTextures(1, &RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, RendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, LocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::Bind(uint32 slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, RendererID));
	}

	void Texture::UnBind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	uint32 TextureManager::LoadTexture(const std::string& FileName)
	{
		auto FindMeshResult = LoadedTextureMap.find(FileName);
		if (FindMeshResult != LoadedTextureMap.end())
		{
			return FindMeshResult->second.GetTexutureID();
		}

		auto Result = LoadedTextureMap.emplace(FileName, Texture());
		if (!Result.second)
		{
			TextureLog().error("Load texture failed: {}", FileName);
			return 0;
		}

		Texture& Texture = Result.first->second;
		Texture.Load(FileName);

		Texture.TextureID = ++TextureID;

		LoadedTextureMapPath[Texture.TextureID] = FileName;

		return Texture.TextureID;
	}

	Texture& TextureManager::GetTexture(uint32 TextureID)
	{
		if (LoadedTextureMapPath.find(TextureID) != LoadedTextureMapPath.end())
		{
			const std::string& filepath = LoadedTextureMapPath.at(TextureID);
			auto FindTextureResult = LoadedTextureMap.find(filepath);
			if (FindTextureResult != LoadedTextureMap.end())
			{
				return FindTextureResult->second;
			}
		}
		return InvalidTexture;
	}

}
