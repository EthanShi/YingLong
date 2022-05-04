#pragma once

#include "glm/glm.hpp"

#include "Camera3D.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace YingLong {

	enum class YINGLONG_API RendererUnit
	{
		Meter,
		Centimeter
	};

	class YINGLONG_API Renderer
	{
	private:
		static glm::mat4 m_ScaleMatrix;

		static uint32 m_windowHeight;
		static uint32 m_windowWidth;

		static ShaderManager m_ShaderManager;

	public:
		// Draw calls
		static void Draw(
			const VertexArray& vao,
			const IndexBuffer& ibo,
			Shader& shader,
			const glm::mat4& modelTransform,
			const glm::mat4& Projection,
			const glm::mat4& View);

		static void Draw(
			const VertexArray& vao,
			const IndexBuffer& ibo,
			uint32 shaderID,
			const glm::mat4& modelTransform,
			const glm::mat4& Projection,
			const glm::mat4& View)
		{
			Draw(vao, ibo, m_ShaderManager.GetShader(shaderID), modelTransform, Projection, View);
		}

		static ShaderManager& GetShaderManager() { return m_ShaderManager; }

		// Set functions
		static void SetClearColor(const glm::vec4& color);
		static void SetClearColor(float red, float green, float blue, float alpha);
		static void Clear();

		static void SetUnit(RendererUnit unit);
		static void SetDepthTestEnable(bool enable);

		// Callbacks
		static void OnWindowSizeChanged(uint32 height, uint32 width);

	private:
		static glm::mat4 GetViewportMatrix();
	};

}
