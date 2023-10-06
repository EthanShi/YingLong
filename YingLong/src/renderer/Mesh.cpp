
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
        MeshObjDataID = Renderer::GetMeshObjDataManager().LoadMeshObjData(FileName, MaterialSearchPath);
        return MeshObjDataID != 0;
	}

    const MeshObjData& Mesh::GetObjData() const
    {
        return Renderer::GetMeshObjDataManager().GetMeshObjData(MeshObjDataID);
    }

    bool Mesh::HasNormals() const
    {
        return GetObjData().HasNormals;
    }

    bool Mesh::HasTexcoords() const
    {
        return GetObjData().HasTexcoords;
    }

    bool Mesh::HasColors() const
    {
        return GetObjData().HasColors;
    }

	void Mesh::FillRenderData(bool withNormal, bool withTexcoord, bool withColor)
	{
        const MeshObjData& ObjData = GetObjData();
        if (!ObjData.ObjReader.Valid())
        {
            MeshLog().error("Fill render data failed, can not get valid ObjData.");
            return;
        }

        auto& attrib = ObjData.ObjReader.GetAttrib();
        auto& shapes = ObjData.ObjReader.GetShapes();

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
                if (ObjData.HasNormals)
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
                if (ObjData.HasTexcoords)
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
                if (ObjData.HasColors)
                {
                    Vertices.insert(
                        Vertices.end(),
                        attrib.colors.begin() + idx.vertex_index * 3,
                        attrib.colors.begin() + idx.vertex_index * 3 + 3
                    );
                }
                else
                {
                    Vertices.push_back(DefaultColor.r);
                    Vertices.push_back(DefaultColor.g);
                    Vertices.push_back(DefaultColor.b);
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

        // Fill VertexBufferLayout
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
        VertexArray.Init();
        VertexBuffer.Init((const float*)Vertices.data(), (uint32)Vertices.size() * sizeof(float), GL_STATIC_DRAW);
        VertexArray.AddBuffer(VertexBuffer, VBOLayout);
        IndexBuffer.Init(Indexes.data(), (uint32)Indexes.size());
	}

    uint32 MeshObjDataManager::LoadMeshObjData(const std::string& FileName, const std::string& materialSearchPath)
    {
        auto FindMeshResult = LoadedMeshObjDataMap.find(FileName);
        if (FindMeshResult != LoadedMeshObjDataMap.end())
        {
            return FindMeshResult->second.MeshObjDataID;
        }

        auto Result = LoadedMeshObjDataMap.emplace(FileName, MeshObjData());
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

        if (!NewObjData.ObjReader.ParseFromFile(FileName, readerConfig)) {
            MeshLog().error("TinyObjReader: {}", NewObjData.ObjReader.Error());
            LoadedMeshObjDataMap.erase(FileName);
            return 0;
        }

        const std::string& WarningMsg = NewObjData.ObjReader.Warning();
        if (!WarningMsg.empty()) {
            MeshLog().warn("TinyObjReader: {}", WarningMsg);
        }

        auto& attrib = NewObjData.ObjReader.GetAttrib();
        NewObjData.HasNormals = !attrib.normals.empty();
        NewObjData.HasTexcoords = !attrib.texcoords.empty();
        NewObjData.HasColors = !attrib.colors.empty();

        NewObjData.MeshObjDataID = ++MeshObjDataID;
        NewObjData.Filename = FileName;

        LoadedMeshObjDataMapPath[NewObjData.MeshObjDataID] = FileName;

        return NewObjData.MeshObjDataID;
    }

    MeshObjData& MeshObjDataManager::GetMeshObjData(uint32 MeshObjDataID)
    {
        if (LoadedMeshObjDataMapPath.find(MeshObjDataID) != LoadedMeshObjDataMapPath.end())
        {
            const std::string& filepath = LoadedMeshObjDataMapPath.at(MeshObjDataID);
            auto FindMeshObjDataResult = LoadedMeshObjDataMap.find(filepath);
            if (FindMeshObjDataResult != LoadedMeshObjDataMap.end())
            {
                return FindMeshObjDataResult->second;
            }
        }
        return InvalidMeshObjData;
    }
}