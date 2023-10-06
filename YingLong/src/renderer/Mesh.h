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

	struct YINGLONG_API MeshObjData
	{
		uint32 MeshObjDataID = 0;
		std::string Filename = "";
		tinyobj::ObjReader ObjReader;
		bool HasNormals = false;
		bool HasTexcoords = false;
		bool HasColors = false;
	};

	class YINGLONG_API Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::string& FileName, const std::string& MaterialSearchPath = std::string());
		~Mesh() {};

		bool LoadObjData(const std::string& FileName, const std::string& MaterialSearchPath = std::string());
		void FillRenderData(bool withNormal, bool withTexcoord, bool withColor);

		void SetDefaultColor(glm::vec3 color) { DefaultColor = color; }
		const glm::vec3& GetDefaultColor() { return DefaultColor; }

		const MeshObjData& GetObjData() const;

		bool HasNormals() const;
		bool HasTexcoords() const;
		bool HasColors() const;

		const VertexArray& GetVertexArray() const { return VertexArray; }
		const IndexBuffer& GetIndexBuffer() const { return IndexBuffer; }

		operator bool() { return GetObjData().ObjReader.Valid(); }

	private:
		uint32 MeshObjDataID = 0;

		glm::vec3 DefaultColor = glm::vec3(0.3, 0.3, 0.3);

		VertexArray VertexArray;
		VertexBuffer VertexBuffer;
		IndexBuffer IndexBuffer;
	};

	class YINGLONG_API MeshObjDataManager
	{
	public:
		uint32 LoadMeshObjData(const std::string& FileName, const std::string& materialSearchPath);
		MeshObjData& GetMeshObjData(uint32 MeshObjDataID);

	private:
		uint32 MeshObjDataID = 0;
		MeshObjData InvalidMeshObjData;
		std::unordered_map<std::string, MeshObjData> LoadedMeshObjDataMap;
		std::unordered_map<uint32, std::string> LoadedMeshObjDataMapPath;
	};

}