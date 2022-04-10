#pragma once

#include <string>
#include <unordered_map>

#include "core/Macros.h"

namespace YingLong {

	struct YINGLONG_API ShaderPrgramSource
	{
		std::string VertexShaderSource;
		std::string FragmentShaderSource;
	};


	class YINGLONG_API Shader
	{
	private:
		uint32 m_RendererID;
		std::string m_Filepath;
		std::unordered_map<std::string, int32> m_UniformLocationCache;

	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void SetUniform1i(const std::string& name, int32 value);
		void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:
		int32 GetUniformLocation(const std::string& name);

		uint32 CompileShader(uint32 type, const std::string& source);
		uint32 CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderPrgramSource ParseShader(const std::string& filepath);
	};

	using Shader_SPtr = std::shared_ptr<Shader>;

}

