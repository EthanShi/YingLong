
#include "YingLongPCH.h"

#include "Renderer3D.h"

namespace YingLong {

	glm::vec4 Renderer::BackgroundColor = glm::vec4();
	float Renderer::UnitScale = 1.0f;
	uint32 Renderer::windowHeight = 2;
	uint32 Renderer::windowWidth = 2;
	ShaderManager Renderer::ShaderManager;
	MeshObjDataManager Renderer::MeshObjDataManager;
	TextureManager Renderer::TextureManager;

	void Renderer::Draw(const VertexArray& vao,
		const IndexBuffer& ibo,
		Shader& shader,
		Shader::Uniforms& Uniforms)
	{
		if (windowHeight == 0 || windowWidth == 0 || shader.GetRendererID() <= 0)
		{
			return;
		}

		shader.Bind(Uniforms);

		shader.SetUniform1f(UNIT_SCALE_UNIFORM_NAME, UnitScale);

		vao.Bind();
		ibo.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::SetBackgroundColor(const glm::vec4& color)
	{
		BackgroundColor = color;
		DrawBackgroundColor();
	}

	void Renderer::DrawBackgroundColor()
	{
		GLCall(glClearColor(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], BackgroundColor[3]));
	}

	const glm::vec4& Renderer::GetBackgroundColor()
	{
		return BackgroundColor;
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetUnit(RendererUnit unit)
	{
		if (unit == RendererUnit::Meter)
		{
			UnitScale = 1.0f;
		}
		else if (unit == RendererUnit::Centimeter)
		{
			UnitScale = 0.01f;
		}
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
		windowHeight = height;
		windowWidth = width;
	}

	glm::mat4 Renderer::GetViewportMatrix()
	{
		glm::mat4 viewport(1.0f);
		viewport = glm::translate(viewport, glm::vec3(-1.0f, -1.0f, 0.0f));
		viewport = glm::scale(viewport, glm::vec3(2.0f / windowWidth, 2.0f / windowHeight, 1.0f));

		return viewport;
	}

}