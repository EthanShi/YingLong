#include "YingLongPCH.h"
#include "PhongMaterial.h"
#include "renderer/Renderer3D.h"

namespace YingLong {

	PhongMaterial::PhongMaterial(const std::string& DiffuseMapFileName, const std::string& SpecularMapFileName, glm::vec3 Specular, float Shininess)
		: Ambient(0.f, 0.f, 0.f)
		, Diffuse(0.f, 0.f, 0.f)
		, Specular(Specular)
		, Shininess(Shininess)
	{
		DiffuseMapTextureID = Renderer::GetTextureManager().LoadTexture(DiffuseMapFileName);
		SpecularMapTextureID = Renderer::GetTextureManager().LoadTexture(SpecularMapFileName);
	}

	Texture& PhongMaterial::GetDiffuseMap()
	{
		return Renderer::GetTextureManager().GetTexture(DiffuseMapTextureID);
	}

	Texture& PhongMaterial::GetSpecularMap()
	{
		return Renderer::GetTextureManager().GetTexture(SpecularMapTextureID);
	}

}
