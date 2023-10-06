#pragma once

#include "glm/glm.hpp"

#include "Camera3D.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

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
			Draw(vao, ibo, ShaderManager.GetShader(shaderID), Uniforms);
		}

		static ShaderManager& GetShaderManager() { return ShaderManager; }
		static MeshObjDataManager& GetMeshObjDataManager() { return MeshObjDataManager; }
		static TextureManager& GetTextureManager() { return TextureManager; }

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
		static glm::vec4 BackgroundColor;

		static float UnitScale;

		static uint32 windowHeight;
		static uint32 windowWidth;

		static ShaderManager ShaderManager;
		static MeshObjDataManager MeshObjDataManager;
		static TextureManager TextureManager;

	private:
		static glm::mat4 GetViewportMatrix();
	};

}
