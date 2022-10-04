#pragma once

#include <string>
#include <unordered_map>

#include "core/Macros.h"
#include "core/Log.h"

DECLARE_LOGGER(ShaderLog)

namespace YingLong {

	struct YINGLONG_API ShaderPrgramSource
	{
		std::string VertexShaderSource;
		std::string FragmentShaderSource;
	};


	class YINGLONG_API Shader
	{
	public:
		struct Uniforms
		{
			void SetUniform(const std::string& name, const int32& value) { m_Uniform1i[name] = value; }
			void SetUniform(const std::string& name, const glm::vec4& value) { m_Uniform4f[name] = value; }
			void SetUniform(const std::string& name, const glm::mat4& value) { m_UniformMat4f[name] = value; }

			void BindUniforms(Shader& shader)
			{
				for (auto& iter : m_Uniform1i)
				{
					shader.SetUniform1i(iter.first, iter.second);
				}
				for (auto& iter : m_Uniform4f)
				{
					shader.SetUniform4f(iter.first, iter.second.x, iter.second.y, iter.second.z, iter.second.w);
				}
				for (auto& iter : m_UniformMat4f)
				{
					shader.SetUniformMat4f(iter.first, iter.second);
				}
			}

		private:
			std::unordered_map<std::string, int32> m_Uniform1i;
			std::unordered_map<std::string, glm::vec4> m_Uniform4f;
			std::unordered_map<std::string, glm::mat4> m_UniformMat4f;
		};

	public:
		Shader() = default;
		Shader(const std::string& filepath);
		~Shader();

		void Load(const std::string& filepath);

		void Bind() const;
		void UnBind() const;

		void SetUniform1i(const std::string& name, int32 value);
		void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

		uint32 GetRendererID() const { return m_RendererID; }

	private:
		int32 GetUniformLocation(const std::string& name);

		uint32 CompileShader(uint32 type, const std::string& source);
		uint32 CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderPrgramSource ParseShader(const std::string& filepath);

	private:
		uint32 m_RendererID = -1;
		std::string m_Filepath = "";
		std::unordered_map<std::string, int32> m_UniformLocationCache;
	};

	using Shader_SPtr = std::shared_ptr<Shader>;

	class YINGLONG_API ShaderManager
	{
	public:
		uint32 LoadShader(const std::string& filepath);
		Shader& GetShader(uint32 shaderID);

	private:
		Shader m_InvalidShader;
		std::unordered_map<std::string, Shader> m_LoadedShaderMap;
		std::unordered_map<uint32, std::string> m_LoadedShaderMapPath;
	};

}

