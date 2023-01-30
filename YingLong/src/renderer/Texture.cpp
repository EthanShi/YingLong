
#include "YingLongPCH.h"

#include "Texture.h"
#include "renderer/Renderer3D.h"

DEFINE_LOGGER(TextureLog)

namespace YingLong {

	Texture::~Texture()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	bool Texture::IsValid() const
	{
		return m_TextureID > 0;
	}

	void Texture::Load(const std::string& filepath)
	{
		m_FilePath = filepath;

		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 3);

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::Bind(uint32 slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}

	void Texture::UnBind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	uint32 TextureManager::LoadTexture(const std::string& FileName)
	{
		auto& FindMeshResult = m_LoadedTextureMap.find(FileName);
		if (FindMeshResult != m_LoadedTextureMap.end())
		{
			return FindMeshResult->second.GetTexutureID();
		}

		auto& Result = m_LoadedTextureMap.emplace(FileName, Texture());
		if (!Result.second)
		{
			TextureLog().error("Load texture failed: {}", FileName);
			return 0;
		}

		Texture& Texture = Result.first->second;
		Texture.Load(FileName);

		Texture.m_TextureID = ++m_TextureID;

		m_LoadedTextureMapPath[Texture.m_TextureID] = FileName;

		return Texture.m_TextureID;
	}

	Texture& TextureManager::GetTexture(uint32 TextureID)
	{
		if (m_LoadedTextureMapPath.find(TextureID) != m_LoadedTextureMapPath.end())
		{
			const std::string& filepath = m_LoadedTextureMapPath.at(TextureID);
			auto& FindTextureResult = m_LoadedTextureMap.find(filepath);
			if (FindTextureResult != m_LoadedTextureMap.end())
			{
				return FindTextureResult->second;
			}
		}
		return m_InvalidTexture;
	}

}
