#pragma once

#include "glm/glm.hpp"

#include "Camera.h"
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

		static std::shared_ptr<Camera> m_Camera;

	public:
		// Draw calls
		static void Draw(const VertexArray& vao, const IndexBuffer& vbo, Shader& shader, glm::mat4 modelTransform);

		// Set functions
		static void SetClearColor(const glm::vec4& color);
		static void SetClearColor(float red, float green, float blue, float alpha);
		static void Clear();

		static void SetCamera(const std::shared_ptr<Camera>& camera);
		static void SetUnit(RendererUnit unit);
		static void SetDepthTestEnable(bool enable);

		// Callbacks
		static void OnWindowSizeChanged(uint32 height, uint32 width);

	private:
		static glm::mat4 GetViewportMatrix();
	};

}
