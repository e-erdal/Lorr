//
// Created on Saturday 16th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/ITexture.hh"
#include "Engine/Graphics/Common/InputLayout.hh"
#include "Engine/Graphics/Common/IRenderBuffer.hh"

#include <obj_loader.h>

namespace Lorr
{
    typedef std::vector<float> OBJVertices;
    typedef std::vector<uint32_t> OBJIndices;
    static InputLayout kMeshLayout = {
        { VertexAttribType::Vec3, "POSITION" },
        { VertexAttribType::Vec3, "NORMAL" },
        { VertexAttribType::Vec2, "TEXCOORD" },
        { VertexAttribType::Vec4, "COLOR" },
    };

    struct MeshVertex
    {
        glm::vec3 Pos;
        glm::vec3 Norm;
        glm::vec2 UV;
        glm::vec4 Color = { 1, 1, 1, 1 };
    };

    class Mesh
    {
    public:
        Mesh() = default;

        void Init(OBJVertices &vertices, OBJIndices &indices, QuickOBJLoader::VertexFormat &format, QuickOBJLoader::Material &material);
        void Init(float radius, uint32_t tessellation, TextureHandle texture);
        void Render();

    public:
        const auto &GetVertices() const
        {
            return m_Vertices;
        }

    private:
        TextureHandle m_Texture = 0;

        RenderBufferHandle m_VertexBuffer = 0;
        RenderBufferHandle m_IndexBuffer = 0;

        std::vector<MeshVertex> m_Vertices;
        uint32_t m_IndexCount = 0;
    };

}  // namespace Lorr