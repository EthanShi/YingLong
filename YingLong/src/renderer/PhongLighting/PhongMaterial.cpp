#include "YingLongPCH.h"
#include "PhongMaterial.h"
#include "renderer/Renderer3D.h"

namespace YingLong {

	PhongMaterial::PhongMaterial(const std::string& DiffuseMapFileName, const std::string& SpecularMapFileName, glm::vec3 Specular, float Shininess)
		: m_Ambient(0.f, 0.f, 0.f)
		, m_Diffuse(0.f, 0.f, 0.f)
		, m_Specular(Specular)
		, m_Shininess(Shininess)
	{
		m_DiffuseMapTextureID = Renderer::GetTextureManager().LoadTexture(DiffuseMapFileName);
		m_SpecularMapTextureID = Renderer::GetTextureManager().LoadTexture(SpecularMapFileName);
	}

	Texture& PhongMaterial::GetDiffuseMap()
	{
		return Renderer::GetTextureManager().GetTexture(m_DiffuseMapTextureID);
	}

	Texture& PhongMaterial::GetSpecularMap()
	{
		return Renderer::GetTextureManager().GetTexture(m_SpecularMapTextureID);
	}

}
