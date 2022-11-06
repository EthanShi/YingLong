#pragma once

#include "glm/glm.hpp"

#include "Camera3D.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Mesh.h"

namespace YingLong {

	enum class YINGLONG_API RendererUnit
	{
		Meter,
		Centimeter
	};

	class YINGLONG_API Renderer
	{
	public:
		// Draw calls
		static void Draw(
			const VertexArray& vao,
			const IndexBuffer& ibo,
			Shader& shader,
			Shader::Uniforms& Uniforms);

		static void Draw(
			const VertexArray& vao,
			const IndexBuffer& ibo,
			uint32 shaderID,
			Shader::Uniforms& Uniforms)
		{
			Draw(vao, ibo, m_ShaderManager.GetShader(shaderID), Uniforms);
		}

		static ShaderManager& GetShaderManager() { return m_ShaderManager; }
		static MeshObjDataManager& GetMeshObjDataManager() { return m_MeshObjDataManager; }

		// Set functions
		static void SetBackgroundColor(const glm::vec4& color);
		static void DrawBackgroundColor();
		static const glm::vec4& GetBackgroundColor();
		static void Clear();

		static void SetUnit(RendererUnit unit);
		static void SetDepthTestEnable(bool enable);

		// Callbacks
		static void OnWindowSizeChanged(uint32 height, uint32 width);

	private:
		// Clear color for renderer
		static glm::vec4 m_BackgroundColor;

		static glm::mat4 m_ScaleMatrix;

		static uint32 m_windowHeight;
		static uint32 m_windowWidth;

		static ShaderManager m_ShaderManager;
		static MeshObjDataManager m_MeshObjDataManager;

	private:
		static glm::mat4 GetViewportMatrix();
	};

}
