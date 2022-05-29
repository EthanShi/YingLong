#include "YingLongPCH.h"
#include "Mesh.h"

namespace YingLong {

    Mesh::Mesh(const std::string& filename, const std::string& materialSearchPath)
	{
		Load(filename, materialSearchPath);
	}

	bool Mesh::Load(const std::string& filename, const std::string& materialSearchPath)
	{
        tinyobj::ObjReaderConfig readerConfig;

        if (materialSearchPath.empty())
        {
            readerConfig.mtl_search_path = std::regex_replace(filename, std::regex("\\\\[a-z]*\.[a-z]*$"), "");
        }
        else
        {
            readerConfig.mtl_search_path = materialSearchPath;
        }

		readerConfig.triangulate = true;

		if (!m_ObjReader.ParseFromFile(filename, readerConfig)) {
			if (!m_ObjReader.Error().empty()) {
				std::cerr << "TinyObjReader: " << m_ObjReader.Error();
			}
            return false;
		}

		if (!m_ObjReader.Warning().empty()) {
			std::cout << "TinyObjReader: " << m_ObjReader.Warning();
		}

        auto& attrib = m_ObjReader.GetAttrib();
        m_HasNormals = !attrib.normals.empty();
        m_HasTexcoords = !attrib.texcoords.empty();
        m_HasColors = !attrib.colors.empty();

        //ASSERT(CheckObjDataValid())

        m_LoadSucceed = true;

        return true;
	}

	void Mesh::FillRenderData(bool withNormal, bool withTexcoord, bool withColor)
	{
        auto& attrib = m_ObjReader.GetAttrib();
        auto& shapes = m_ObjReader.GetShapes();

        std::vector<float> Vertices;
        std::vector<uint32> Indexes;
        std::unordered_map<std::string, size_t> IndexMap;

        auto GetIndexInVertices = [this, withNormal, withTexcoord, withColor, &attrib, &Vertices, &IndexMap](tinyobj::index_t idx) -> size_t
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
                if (m_HasNormals)
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
                if (m_HasTexcoords)
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
                if (m_HasColors)
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
}