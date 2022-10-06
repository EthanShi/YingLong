#pragma once

#include "glm/glm.hpp"

#include "renderer/Mesh.h"
#include "renderer/Shader.h"
#include "renderer/Renderer3D.h"

using namespace YingLong;

struct MeshComponent
{
	Mesh& MeshRef;

	MeshComponent(Mesh& InMesh)
		: MeshRef(InMesh)
	{}

	MeshComponent& operator=(const MeshComponent& Other)
	{
		if (&Other != this)
		{
			MeshRef = Other.MeshRef;
		}
		return *this;
	}
};

struct ShaderComponent
{
	uint32 shaderID;
	Shader::Uniforms uniforms;

	void LoadShader(const std::string& FileName)
	{
		shaderID = Renderer::GetShaderManager().LoadShader(FileName);
	}
};