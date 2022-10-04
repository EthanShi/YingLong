#pragma once

#include <string>

#include "glm/glm.hpp"
#include "tinyobjloader/tiny_obj_loader.h"

#include "core/Macros.h"
#include "core/Log.h"

#include "renderer/VertexArray.h"
#include "renderer/VertexBuffer.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/IndexBuffer.h"

DECLARE_LOGGER(MeshLog)

namespace YingLong {

	class YINGLONG_API Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::string& filename, const std::string& materialSearchPath = std::string());
		~Mesh() {};

		bool Load(const std::string& filename, const std::string& materialSearchPath = std::string());
		void FillRenderData(bool withNormal, bool withTexcoord, bool withColor);

		void SetDefaultColor(glm::vec3 color) { m_DefaultColor = color; }
		const glm::vec3& GetDefaultColor() { return m_DefaultColor; }

		bool HasNormals() const { return m_HasNormals; }
		bool HasTexcoords() const { return m_HasTexcoords; }
		bool HasColors() const { return m_HasColors; }

		VertexArray& GetVertexArray() { return m_VertexArray; }
		IndexBuffer& GetIndexBuffer() { return m_IndexBuffer; }

	private:
		bool m_LoadSucceed = false;
		std::string m_Filename = "";
		tinyobj::ObjReader m_ObjReader;
		bool m_HasNormals = false;
		bool m_HasTexcoords = false;
		bool m_HasColors = false;

		glm::vec3 m_DefaultColor = glm::vec3(0.3, 0.3, 0.3);

		VertexArray m_VertexArray;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
	};

}