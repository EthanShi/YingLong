#pragma once

#include "glm/glm.hpp"

#include "renderer/Mesh.h"
#include "renderer/Shader.h"
#include "renderer/Renderer3D.h"

namespace YingLong {

	struct MeshComponent
	{
		Mesh& m_MeshRef;

		MeshComponent(Mesh& InMesh)
			: m_MeshRef(InMesh)
		{}

		MeshComponent& operator=(const MeshComponent& Other)
		{
			if (&Other != this)
			{
				m_MeshRef = Other.m_MeshRef;
			}
			return *this;
		}
	};

	struct ShaderComponent
	{
		uint32 m_ShaderID;
		Shader::Uniforms m_Uniforms;

		void LoadShader(const std::string& FileName)
		{
			m_ShaderID = Renderer::GetShaderManager().LoadShader(FileName);
		}
	};

}
