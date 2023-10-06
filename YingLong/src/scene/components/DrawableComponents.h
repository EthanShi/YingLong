#pragma once

#include "glm/glm.hpp"

#include "renderer/Mesh.h"
#include "renderer/Shader.h"
#include "renderer/Renderer3D.h"

namespace YingLong {

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
		uint32 ShaderID;
		Shader::Uniforms Uniforms;

		void LoadShader(const std::string& FileName)
		{
			ShaderID = Renderer::GetShaderManager().LoadShader(FileName);
		}
	};

}
