
#include "YingLongPCH.h"

#include "Renderer.h"

namespace YingLong {

	glm::mat4 Renderer::m_ScaleMatrix = glm::mat4(1.0);
	uint32 Renderer::m_windowHeight = 2;
	uint32 Renderer::m_windowWidth = 2;
	std::shared_ptr<Camera> Renderer::m_Camera = nullptr;

	void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, Shader& shader, glm::mat4 modelTransform)
	{
		if (m_windowHeight == 0 || m_windowWidth == 0 || !m_Camera)
		{
			return;
		}

		shader.Bind();
		shader.SetUniformMat4f(VIEWPORT_UNIFORM_NAME, GetViewportMatrix());
		glm::mat4 Proj = m_Camera->GetPerspective();
		glm::mat4 View = m_Camera->GetInverseTransform();

		shader.SetUniformMat4f(MVP_UNIFORM_NAME, Proj * View * modelTransform * m_ScaleMatrix);

		vao.Bind();
		ibo.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::SetClearColor(float red, float green, float blue, float alpha)
	{
		GLCall(glClearColor(red, green, blue, alpha));
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetCamera(const std::shared_ptr<Camera>& camera)
	{
		m_Camera = camera;
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