#pragma once

#include "glm/glm.hpp"

#include "renderer/Mesh.h"
#include "renderer/Shader.h"
#include "renderer/Renderer3D.h"

using namespace YingLong;

struct MeshComponent
{
	Mesh mesh;
};

struct ShaderComponent
{
	uint32 shaderID;
	Shader::Uniforms uniforms;

	void LoadShader(const std::string& filepath)
	{
		shaderID = Renderer::GetShaderManager().LoadShader(filepath);
	}
};