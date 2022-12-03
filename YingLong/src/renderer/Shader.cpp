
#include "YingLongPCH.h"

#include "Shader.h"
#include "RendererUtils.h"

DEFINE_LOGGER(ShaderLog)

namespace YingLong {

	Shader::Shader(const std::string& filepath)
	{
		Load(filepath);
	}

	Shader::~Shader()
	{
	}

	void Shader::Load(const std::string& filepath)
	{
		m_Filepath = filepath;

		ShaderPrgramSource source = ParseShader(m_Filepath);
		if (source.FragmentShaderSource.empty() || source.VertexShaderSource.empty())
		{
			ShaderLog().error("Can not find shader: '{}'", filepath);
			return;
		}
		m_RendererID = CreateShader(source.VertexShaderSource, source.FragmentShaderSource);

		if (m_RendererID == 0)
		{
			ShaderLog().error("Failed to load shader: '{}'", filepath);
		}
	}

	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}

	void Shader::Bind(const Uniforms& InUniforms)
	{
		Bind();
		InUniforms.BindUniforms(*this);
	}

	void Shader::UnBind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::SetUniform1i(const std::string& name, int32 value)
	{
		GLCall(glUniform1i(GetUniformLocation(name), value));
	}

	void Shader::SetUniform1f(const std::string& name, float value)
	{
		GLCall(glUniform1f(GetUniformLocation(name), value));
	}

	void Shader::SetUniform3f(const std::string& name, float f0, float f1, float f2)
	{
		GLCall(glUniform3f(GetUniformLocation(name), f0, f1, f2));
	}

	void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
	{
		GLCall(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

	int32 Shader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		{
			return m_UniformLocationCache[name];
		}

		GLCall(int32 location = glGetUniformLocation(m_RendererID, name.c_str()));
		if (location == -1)
		{
			ShaderLog().warn("Warning: uniform '{}' doesn't exist!", name);
		}

		m_UniformLocationCache[name] = location;
		return location;
	}

	uint32 Shader::CompileShader(uint32 type, const std::string& source)
	{
		GLCall(uint32 id = glCreateShader(type));
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		int32 result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int32 length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			ShaderLog().error("Failed to compile {}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
			ShaderLog().error(message);
			GLCall(glDeleteShader(id));
			return 0;
		}

		return id;
	}

	uint32 Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLCall(uint32 program = glCreateProgram());
		uint32 vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		uint32 fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		if (vs == 0 || fs == 0)
		{
			GLCall(glDeleteProgram(program));
			return 0;
		}

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));

		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDetachShader(program, vs));
		GLCall(glDetachShader(program, fs));

		return program;
	}

	ShaderPrgramSource Shader::ParseShader(const std::string& filepath)
	{
		std::ifstream stream(filepath);

		std::string line;
		std::stringstream vs;
		std::stringstream fs;
		std::stringstream* cur = &vs;
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					cur = &vs;
				}
				else
				{
					cur = &fs;
				}
			}
			else
			{
				*cur << line << '\n';
			}
		}

		return { vs.str(), fs.str() };
	}

	uint32 ShaderManager::LoadShader(const std::string& filepath)
	{
		auto& shader = m_LoadedShaderMap.find(filepath);
		if (shader != m_LoadedShaderMap.end())
		{
			return shader->second.GetRendererID();
		}

		auto& result = m_LoadedShaderMap.emplace(filepath, Shader(filepath));
		if (result.second)
		{

			uint32 rendererID = result.first->second.GetRendererID();
			m_LoadedShaderMapPath.emplace(rendererID, filepath);
			return rendererID;
		}
		return 0;
	}

	Shader& ShaderManager::GetShader(uint32 shaderID)
	{
		if (m_LoadedShaderMapPath.find(shaderID) != m_LoadedShaderMapPath.end())
		{
			const std::string& filepath = m_LoadedShaderMapPath.at(shaderID);
			if (m_LoadedShaderMap.find(filepath) != m_LoadedShaderMap.end())
			{
				return m_LoadedShaderMap.at(filepath);
			}
		}
		return m_InvalidShader;
	}

}
