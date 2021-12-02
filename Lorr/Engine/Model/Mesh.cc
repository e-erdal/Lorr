#include "Mesh.hh"

#include "Engine/App/Engine.hh"

#include <bvh/triangle.hpp>
#include <bvh/sweep_sah_builder.hpp>

namespace Lorr
{
    InputLayout Mesh::m_Layout = {
        { VertexAttribType::Vec3, "POSITION" },
        { VertexAttribType::Vec3, "NORMAL" },
        { VertexAttribType::Vec2, "TEXCOORD" },
        { VertexAttribType::Vec4, "COLOR" },
    };

    // *** BVH Helpers ***
    using Triangle = bvh::Triangle<Mesh::Scalar>;
    using Ray = bvh::Ray<Mesh::Scalar>;
    using _vec3 = bvh::Vector3<Mesh::Scalar>;

    void Mesh::Init(std::vector<FileMeshVertex> &vertices, std::vector<u32> &indices)
    {
        ZoneScoped;

        m_IndexCount = indices.size();

        m_Texture = GetEngine()->GetRenderer()->GetPlaceholder();

        m_Vertices.resize(vertices.size());
        MeshVertex *pStartVertex = &m_Vertices[0];
        for (auto &vertex : vertices)
        {
            pStartVertex->Pos = vertex.Pos;
            pStartVertex->Norm = vertex.Norm;
            pStartVertex->UV = vertex.UV;
            pStartVertex->Color = { 1, 1, 1, 1 };
            pStartVertex++;
        }

        RenderBufferDesc vertexDesc;
        vertexDesc.pData = &m_Vertices[0];
        vertexDesc.DataLen = m_Vertices.size() * sizeof(MeshVertex);
        vertexDesc.Type = RenderBufferType::Vertex;

        m_VertexBuffer = RenderBuffer::Create(vertexDesc);

        RenderBufferDesc indexDesc;
        indexDesc.pData = &indices[0];
        indexDesc.DataLen = indices.size() * sizeof(u32);
        indexDesc.Type = RenderBufferType::Index;

        m_IndexBuffer = RenderBuffer::Create(indexDesc);

        // Init BVH
        std::vector<Triangle> triangles;
        triangles.resize(vertices.size() / 3);

        for (size_t i = 0; i < vertices.size() / 3; i += 3)
        {
            const auto &v0 = vertices[i + 0];
            const auto &v1 = vertices[i + 1];
            const auto &v2 = vertices[i + 2];
            triangles[i] = (Triangle(_vec3(v0.Pos.x, v0.Pos.y, v0.Pos.z), _vec3(v1.Pos.x, v1.Pos.y, v1.Pos.z), _vec3(v2.Pos.x, v2.Pos.y, v2.Pos.z)));
        }

        auto [bboxes, centers] = bvh::compute_bounding_boxes_and_centers(triangles.data(), triangles.size());
        auto globalBBox = bvh::compute_bounding_boxes_union(bboxes.get(), triangles.size());
        m_pBVH = new BVH;
        bvh::SweepSahBuilder<BVH> builder(*m_pBVH);
        builder.build(globalBBox, bboxes.get(), centers.get(), triangles.size());
    }

    void Mesh::Init(float radius, u32 tessellation, TextureHandle texture)
    {
        ZoneScoped;

        m_Texture = texture;

        std::vector<MeshVertex> vertexArray;
        std::vector<u32> indexArray;

        if (tessellation < 3) return;

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
        indexDesc.DataLen = indexArray.size() * sizeof(u32);
        indexDesc.Type = RenderBufferType::Index;

        m_IndexBuffer = RenderBuffer::Create(indexDesc);
    }

    void Mesh::Render()
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        m_Texture->Use();
        pRenderer->UseVertexBuffer(m_VertexBuffer, &m_Layout);
        pRenderer->UseIndexBuffer(m_IndexBuffer);

        pRenderer->DrawIndexed(m_IndexCount);
    }

}  // namespace Lorr