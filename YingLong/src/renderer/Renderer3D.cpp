
#include "YingLongPCH.h"

#include "Renderer3D.h"

namespace YingLong {

	glm::vec4 Renderer::m_BackgroundColor = glm::vec4();
	glm::mat4 Renderer::m_ScaleMatrix = glm::mat4(1.0);
	uint32 Renderer::m_windowHeight = 2;
	uint32 Renderer::m_windowWidth = 2;
	ShaderManager Renderer::m_ShaderManager;
	MeshObjDataManager Renderer::m_MeshObjDataManager;

	void Renderer::Draw(const VertexArray& vao,
		const IndexBuffer& ibo,
		Shader& shader,
		const glm::mat4& modelTransform,
		const glm::mat4& Projection,
		const glm::mat4& View)
	{
		if (m_windowHeight == 0 || m_windowWidth == 0 || shader.GetRendererID() <= 0)
		{
			return;
		}

		shader.Bind();

		shader.SetUniformMat4f(MVP_UNIFORM_NAME, Projection * View * modelTransform * m_ScaleMatrix);

		vao.Bind();
		ibo.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::SetBackgroundColor(const glm::vec4& color)
	{
		m_BackgroundColor = color;
		DrawBackgroundColor();
	}

	void Renderer::DrawBackgroundColor()
	{
		GLCall(glClearColor(m_BackgroundColor[0], m_BackgroundColor[1], m_BackgroundColor[2], m_BackgroundColor[3]));
	}

	const glm::vec4& Renderer::GetBackgroundColor()
	{
		return m_BackgroundColor;
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetUnit(RendererUnit unit)
	{
		float scale = 1.0f;
		if (unit == RendererUnit::Meter)
		{
			scale = 1.0f;
		}
		else if (unit == RendererUnit::Centimeter)
		{
			scale = 0.01f;
		}
		m_ScaleMatrix = glm::mat4(1.0f);
		m_ScaleMatrix = glm::scale(m_ScaleMatrix, glm::vec3(scale, scale, scale));
	}

	void Renderer::SetDepthTestEnable(bool enable)
	{
		if (enable)
		{
			GLCall(glEnable(GL_DEPTH_TEST));
		}
		else
		{
			GLCall(glDisable(GL_DEPTH_TEST));
		}
	}

	void Renderer::OnWindowSizeChanged(uint32 width, uint32 height)
	{
		m_windowHeight = height;
		m_windowWidth = width;
	}

	glm::mat4 Renderer::GetViewportMatrix()
	{
		glm::mat4 viewport(1.0f);
		viewport = glm::translate(viewport, glm::vec3(-1.0f, -1.0f, 0.0f));
		viewport = glm::scale(viewport, glm::vec3(2.0f / m_windowWidth, 2.0f / m_windowHeight, 1.0f));

		return viewport;
	}

}