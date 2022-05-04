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

        ASSERT(CheckObjDataValid())

        m_LoadSucceed = true;

        return true;
	}

	void Mesh::FillRenderData(bool withNormal, bool withTexcoord, bool withColor)
	{
        if (!m_LoadSucceed) return;

        auto& attrib = m_ObjReader.GetAttrib();
        auto& shapes = m_ObjReader.GetShapes();
        auto& materials = m_ObjReader.GetMaterials();

        size_t VertexBufferSize = 0;
        size_t IndexBufferSize = 0;
        CalcBufferSize(VertexBufferSize, IndexBufferSize);
        std::vector<float> Vertices;
        Vertices.reserve(VertexBufferSize);
        std::vector<uint32> Indexes;
        Indexes.reserve(IndexBufferSize);

        // fill Vertices
        size_t verticesCount = attrib.vertices.size() / 3;
        for (size_t idx = 0; idx < verticesCount; idx++)
        {
            Vertices.insert(
                Vertices.end(),
                attrib.vertices.begin() + idx * 3,
                attrib.vertices.begin() + idx * 3 + 3
            );

            if (withNormal)
            {
                if (m_HasNormals)
                {
                    Vertices.insert(
                        Vertices.end(),
                        attrib.normals.begin() + idx * 3,
                        attrib.vertices.begin() + idx * 3 + 3
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
                        attrib.texcoords.begin() + idx * 2,
                        attrib.texcoords.begin() + idx * 2 + 2
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
                        attrib.colors.begin() + idx * 3,
                        attrib.colors.begin() + idx * 3 + 3
                    );
                }
                else
                {
                    Vertices.push_back(m_DefaultColor.r);
                    Vertices.push_back(m_DefaultColor.g);
                    Vertices.push_back(m_DefaultColor.b);
                }
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

        // Loop over shapes, fill Indexes
        for (size_t s = 0; s < shapes.size(); s++) {
            for (auto& index : shapes[s].mesh.indices)
            {
                Indexes.push_back(index.vertex_index);
            }
        }

        // Fill render datas
        m_VertexArray.Init();
        m_VertexBuffer.Init((const float*)Vertices.data(), (uint32)Vertices.size() * sizeof(float), GL_STATIC_DRAW);
        m_VertexArray.AddBuffer(m_VertexBuffer, VBOLayout);
        m_IndexBuffer.Init(Indexes.data(), (uint32)Indexes.size());
	}

    void Mesh::CalcBufferSize(size_t& vertexBufferSize, size_t& indexBufferSize)
    {
        if (!m_LoadSucceed)
        {
            vertexBufferSize = 0;
            indexBufferSize = 0;
            return;
        }

        auto& attrib = m_ObjReader.GetAttrib();
        auto& shapes = m_ObjReader.GetShapes();
        auto& materials = m_ObjReader.GetMaterials();

        size_t SizePerVertice = 3;
        SizePerVertice += m_HasNormals ? 3 : 0;
        if (m_HasTexcoords)
        {
            SizePerVertice += 2;
        }
        else if (m_HasColors)
        {
            SizePerVertice += 3;
        }

        vertexBufferSize = SizePerVertice * attrib.vertices.size();

        indexBufferSize = 0;
        for (size_t s = 0; s < shapes.size(); s++) {
            indexBufferSize += shapes[s].mesh.indices.size();
        }
    }

    bool Mesh::CheckObjDataValid()
    {
        auto& attrib = m_ObjReader.GetAttrib();
        auto& shapes = m_ObjReader.GetShapes();

        // Check if vertice/normal/texcoord/color one-to-one correspondence
        size_t verticesCount = attrib.vertices.size() / 3;
        size_t normalsCount = attrib.normals.size() / 3;
        size_t texcoordsCount = attrib.texcoords.size() / 3;
        size_t colorsCount = attrib.colors.size() / 3;
        if (verticesCount == 0
            || ((normalsCount != 0) && normalsCount != verticesCount)
            || ((texcoordsCount != 0) && texcoordsCount != verticesCount)
            || ((colorsCount != 0) && colorsCount != verticesCount))
        {
            std::cout << "[Load Mesh Error] " << m_Filename << "\n"
                << "Only support vertice/normal/texcoord/color one-to-one correspondence if they exist. " << " ("
                << "verticesCount: " << verticesCount << ", "
                << "normalsCount: " << normalsCount << ", "
                << "colorsCount: " << colorsCount << ", "
                << "texcoordsCount: " << texcoordsCount << ")" << std::endl;
            return false;
        }

        // Check if indexes are all same per vertices
        for (size_t s = 0; s < shapes.size(); s++) {
            size_t index_offset = 0;
            for (auto& index : shapes[s].mesh.indices)
            {
                if ((index.normal_index >= 0 && index.normal_index != index.vertex_index) ||
                    (index.texcoord_index >= 0 && index.texcoord_index != index.vertex_index))
                {
                    std::cout << "[Load Mesh Error] " << m_Filename << "\n"
                        << "Found indices's normal_index/texcoord_index are not equal to vertex_index. " << "("
                        << "vertex_index: " << index.vertex_index << ", "
                        << "normal_index: " << index.normal_index << ", "
                        << "texcoord_index: " << index.texcoord_index << ")" << std::endl;
                    return false;
                }
            }
        }
        
        return true;
    }
}