
#include "YingLongPCH.h"

#include "Mesh.h"
#include "utils/Path.h"
#include "renderer/Renderer3D.h"

DEFINE_LOGGER(MeshLog)

namespace YingLong {

    Mesh::Mesh(const std::string& FileName, const std::string& materialSearchPath)
	{
        LoadObjData(FileName, materialSearchPath);
	}

	bool Mesh::LoadObjData(const std::string& FileName, const std::string& MaterialSearchPath)
	{
        m_MeshObjDataID = Renderer::GetMeshObjDataManager().LoadMeshObjData(FileName, MaterialSearchPath);
        return m_MeshObjDataID != 0;
	}

    const MeshObjData& Mesh::GetObjData() const
    {
        return Renderer::GetMeshObjDataManager().GetMeshObjData(m_MeshObjDataID);
    }

    bool Mesh::HasNormals() const
    {
        return GetObjData().m_HasNormals;
    }

    bool Mesh::HasTexcoords() const
    {
        return GetObjData().m_HasTexcoords;
    }

    bool Mesh::HasColors() const
    {
        return GetObjData().m_HasColors;
    }

	void Mesh::FillRenderData(bool withNormal, bool withTexcoord, bool withColor)
	{
        const MeshObjData& ObjData = GetObjData();
        if (!ObjData.m_ObjReader.Valid())
        {
            MeshLog().error("Fill render data failed, can not get valid ObjData.");
            return;
        }

        auto& attrib = ObjData.m_ObjReader.GetAttrib();
        auto& shapes = ObjData.m_ObjReader.GetShapes();

        std::vector<float> Vertices;
        std::vector<uint32> Indexes;
        std::unordered_map<std::string, size_t> IndexMap;

        auto GetIndexInVertices = [this, withNormal, withTexcoord, withColor, &ObjData, &attrib, &Vertices, &IndexMap](tinyobj::index_t idx) -> size_t
        {
            int vi = idx.vertex_index;
            int ni = withNormal ? idx.normal_index : -1;
            int ti = withTexcoord ? idx.texcoord_index : -1;
            std::string key = std::to_string(vi) + "," + std::to_string(ni) + "," + std::to_string(ti);

            auto iterOfKey = IndexMap.find(key);
            if (iterOfKey != IndexMap.end())
            {
                return iterOfKey->second;
            }

            size_t sizePerVertex = 3;
            sizePerVertex += withNormal ? 3 : 0;
            sizePerVertex += withTexcoord ? 2 : 0;
            sizePerVertex += withColor ? 3 : 0;
            size_t currentCount = Vertices.size() / sizePerVertex;

            IndexMap[key] = currentCount;

            Vertices.insert(
                Vertices.end(),
                attrib.vertices.begin() + idx.vertex_index * 3,
                attrib.vertices.begin() + idx.vertex_index * 3 + 3
            );

            if (withNormal)
            {
                if (ObjData.m_HasNormals)
                {
                    Vertices.insert(
                        Vertices.end(),
                        attrib.normals.begin() + idx.normal_index * 3,
                        attrib.normals.begin() + idx.normal_index * 3 + 3
                    );
                }
                else
                {
                    Vertices.push_back(0.f);
                    Vertices.push_back(0.f);
                    Vertices.push_back(0.f);
                }
            }

            if (withTexcoord)
            {
                if (ObjData.m_HasTexcoords)
                {
                    Vertices.insert(
                        Vertices.end(),
                        attrib.texcoords.begin() + idx.texcoord_index * 2,
                        attrib.texcoords.begin() + idx.texcoord_index * 2 + 2
                    );
                }
                else
                {
                    Vertices.push_back(0.f);
                    Vertices.push_back(0.f);
                }
            }

            if (withColor)
            {
                if (ObjData.m_HasColors)
                {
                    Vertices.insert(
                        Vertices.end(),
                        attrib.colors.begin() + idx.vertex_index * 3,
                        attrib.colors.begin() + idx.vertex_index * 3 + 3
                    );
                }
                else
                {
                    Vertices.push_back(m_DefaultColor.r);
                    Vertices.push_back(m_DefaultColor.g);
                    Vertices.push_back(m_DefaultColor.b);
                }
            }

            return currentCount;
        };

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            for (auto& index : shapes[s].mesh.indices)
            {
                uint32 idxInVertices = (uint32)GetIndexInVertices(index);
                Indexes.push_back(idxInVertices);
            }
        }

        // Fill m_VertexBufferLayout
        VertexBufferLayout VBOLayout;
        VBOLayout.Push<float>(3);
        if (withNormal)
        {
            VBOLayout.Push<float>(3);
        }
        if (withTexcoord)
        {
            VBOLayout.Push<float>(2);
        }
        if (withColor)
        {
            VBOLayout.Push<float>(3);
        }

        // Fill render datas
        m_VertexArray.Init();
        m_VertexBuffer.Init((const float*)Vertices.data(), (uint32)Vertices.size() * sizeof(float), GL_STATIC_DRAW);
        m_VertexArray.AddBuffer(m_VertexBuffer, VBOLayout);
        m_IndexBuffer.Init(Indexes.data(), (uint32)Indexes.size());
	}

    uint32 MeshObjDataManager::LoadMeshObjData(const std::string& FileName, const std::string& materialSearchPath)
    {
        auto& FindMeshResult = m_LoadedMeshObjDataMap.find(FileName);
        if (FindMeshResult != m_LoadedMeshObjDataMap.end())
        {
            return FindMeshResult->second.m_MeshObjDataID;
        }

        auto& Result = m_LoadedMeshObjDataMap.emplace(FileName, MeshObjData());
        if (!Result.second)
        {
            MeshLog().error("Load mesh failed: {}", FileName);
            return 0;
        }

        MeshObjData& NewObjData = Result.first->second;

        tinyobj::ObjReaderConfig readerConfig;

        if (materialSearchPath.empty())
        {
            readerConfig.mtl_search_path = Path::GetDirFromFileName(FileName);
        }
        else
        {
            readerConfig.mtl_search_path = materialSearchPath;
        }

        readerConfig.triangulate = true;

        if (!NewObjData.m_ObjReader.ParseFromFile(FileName, readerConfig)) {
            MeshLog().error("TinyObjReader: {}", NewObjData.m_ObjReader.Error());
            m_LoadedMeshObjDataMap.erase(FileName);
            return 0;
        }

        const std::string& WarningMsg = NewObjData.m_ObjReader.Warning();
        if (!WarningMsg.empty()) {
            MeshLog().warn("TinyObjReader: {}", WarningMsg);
        }

        auto& attrib = NewObjData.m_ObjReader.GetAttrib();
        NewObjData.m_HasNormals = !attrib.normals.empty();
        NewObjData.m_HasTexcoords = !attrib.texcoords.empty();
        NewObjData.m_HasColors = !attrib.colors.empty();

        NewObjData.m_MeshObjDataID = ++m_MeshObjDataID;
        NewObjData.m_Filename = FileName;

        m_LoadedMeshObjDataMapPath[NewObjData.m_MeshObjDataID] = FileName;

        return NewObjData.m_MeshObjDataID;
    }

    MeshObjData& MeshObjDataManager::GetMeshObjData(uint32 MeshObjDataID)
    {
        if (m_LoadedMeshObjDataMapPath.find(MeshObjDataID) != m_LoadedMeshObjDataMapPath.end())
        {
            const std::string& filepath = m_LoadedMeshObjDataMapPath.at(MeshObjDataID);
            auto& FindMeshObjDataResult = m_LoadedMeshObjDataMap.find(filepath);
            if (FindMeshObjDataResult != m_LoadedMeshObjDataMap.end())
            {
                return FindMeshObjDataResult->second;
            }
        }
        return m_InvalidMeshObjData;
    }
}