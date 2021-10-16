#include "Mesh.hh"

namespace Lorr
{
    void Mesh::Init(OBJVertices &vertices, uint32_t indices, QuickOBJLoader::VertexFormat &format, QuickOBJLoader::Material &material)
    {
        size_t offset = 0;

        while (offset != vertices.size())
        {
            MeshVertex vertex{};

            if (format.positionElementOffset.has_value())
            {
                float x = vertices[offset + 0];
                float y = vertices[offset + 1];
                float z = vertices[offset + 2];
                offset += 3;

                vertex.Pos = { x, y, z };
            }

            if (format.normalElementOffset.has_value())
            {
                float x = vertices[offset + 0];
                float y = vertices[offset + 1];
                float z = vertices[offset + 2];
                offset += 3;

                vertex.Norm = { x, y, z };
            }

            if (format.textureCoordinatesElementOffset.has_value())
            {
                float u = vertices[offset + 0];
                float v = vertices[offset + 1];
                offset += 2;

                // vertex.UV = { u, v };
            }

            vertex.Color = { material.R, material.G, material.B, 1.f };
            m_Vertices.push_back(vertex);
        }

        m_IndexOffset = indices;
    }

}  // namespace Lorr