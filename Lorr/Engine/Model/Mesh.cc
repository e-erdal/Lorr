#include "Mesh.hh"

#include "Engine/App/Engine.hh"

namespace Lorr
{
    void Mesh::Init(OBJVertices &vertices, OBJIndices &indices, QuickOBJLoader::VertexFormat &format, QuickOBJLoader::Material &material)
    {
        ZoneScoped;
        
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

                vertex.UV = { u, 1 - v };
            }

            vertex.Color = { material.DiffuseR, material.DiffuseG, material.DiffuseB, 1.f };
            m_Vertices.push_back(vertex);
        }

        m_IndexCount = indices.size();

        if (material.DiffuseTexture != "")
        {
            TextureDesc desc;
            desc.MipMapLevels = 4;
            // RenderBufferUsage::Dynamic, RB_ACCESS_TYPE_CPUW
            m_Texture = Texture::Create(fmt::format("model://{}", material.DiffuseTexture), material.DiffuseTexture + ".lr", &desc);
        }
        else
        {
            m_Texture = GetEngine()->GetRenderer()->GetPlaceholder();
        }

        RenderBufferDesc vertexDesc;
        vertexDesc.pData = &m_Vertices[0];
        vertexDesc.DataLen = m_Vertices.size() * sizeof(MeshVertex);
        vertexDesc.Type = RenderBufferType::Vertex;

        m_VertexBuffer = RenderBuffer::Create(vertexDesc);

        RenderBufferDesc indexDesc;
        indexDesc.pData = &indices[0];
        indexDesc.DataLen = indices.size() * sizeof(uint32_t);
        indexDesc.Type = RenderBufferType::Index;

        m_IndexBuffer = RenderBuffer::Create(indexDesc);
    }

    void Mesh::Init(float radius, uint32_t tessellation, TextureHandle texture)
    {
        ZoneScoped;
        
        m_Texture = texture;

        std::vector<MeshVertex> vertexArray;
        std::vector<uint32_t> indexArray;

        if (tessellation < 3)
        {
            return;
        }

        int segVert = tessellation;
        int segHor = tessellation * 2;

        for (size_t i = 0; i <= segVert; i++)
        {
            float v = 1 - (float)i / segVert;

            float latitude = (i * (Math::PI2) / segVert) - Math::PIDIV2;
            float dy;
            float dxz;

            Math::ScalarSinCos(&dy, &dxz, latitude);

            // Single latitude ring for horizontal segment
            for (size_t e = 0; e <= segHor; e++)
            {
                float u = 1 - (float)i / segHor;

                float longitude = e * Math::PI2 / segHor;
                float dx;
                float dz;

                Math::ScalarSinCos(&dx, &dz, longitude);

                dx *= dxz;
                dz *= dxz;

                glm::vec3 normal(dx, dy, dz);
                glm::vec2 uv(u, v);

                MeshVertex vert = {};
                vert.Pos = normal * radius;
                vert.Norm = normal;
                vert.UV = uv;
                vert.Color = { 1.f, 1.f, 1.f, 1.f };

                vertexArray.push_back(vert);
            }
        }

        int stride = segHor + 1;

        for (int x = 0; x < segVert; x++)
        {
            for (int y = 0; y < segHor; y++)
            {
                int nextX = x + 1;
                int nextY = (y + 1) % stride;

                indexArray.resize(indexArray.size() + 6);

                indexArray.push_back(x * stride + nextY);
                indexArray.push_back(nextX * stride + y);
                indexArray.push_back(x * stride + y);

                indexArray.push_back(nextX * stride + nextY);
                indexArray.push_back(nextX * stride + y);
                indexArray.push_back(x * stride + nextY);
            }
        }

        m_IndexCount = indexArray.size();

        RenderBufferDesc vertexDesc;
        vertexDesc.pData = &vertexArray[0];
        vertexDesc.DataLen = vertexArray.size() * sizeof(MeshVertex);
        vertexDesc.Type = RenderBufferType::Vertex;

        m_VertexBuffer = RenderBuffer::Create(vertexDesc);

        RenderBufferDesc indexDesc;
        indexDesc.pData = &indexArray[0];
        indexDesc.DataLen = indexArray.size() * sizeof(uint32_t);
        indexDesc.Type = RenderBufferType::Index;

        m_IndexBuffer = RenderBuffer::Create(indexDesc);
    }

    void Mesh::Render()
    {
        ZoneScoped;
        
        IRenderer *pRenderer = GetEngine()->GetRenderer();

        m_Texture->Use();
        pRenderer->UseVertexBuffer(m_VertexBuffer, &kMeshLayout);
        pRenderer->UseIndexBuffer(m_IndexBuffer);

        GetEngine()->GetRenderer()->DrawIndexed(m_IndexCount);
    }

}  // namespace Lorr