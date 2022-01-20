//
// Created on Saturday 16th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/BaseTexture.hh"
#include "Engine/Graphics/Common/BaseRenderBuffer.hh"
#include "Engine/Graphics/Common/InputLayout.hh"

namespace lr
{
    struct FileMeshVertex
    {
        glm::vec3 Pos = {};
        glm::vec3 Norm = {};
        glm::vec2 UV = { FLT_MAX, FLT_MAX };
    };

    struct FileMesh
    {
        eastl::vector<FileMeshVertex> Vertices;
        eastl::vector<u32> Indices;
    };

    struct MeshVertex
    {
        glm::vec3 Pos = {};
        glm::vec3 Norm = {};
        glm::vec2 UV = {};
        glm::vec4 Color = { 1, 1, 1, 1 };
    };

    class Mesh
    {
    public:
        Mesh() = default;

        void Init(eastl::vector<FileMeshVertex> &vertices, eastl::vector<u32> &indices);
        void Init(float radius, u32 tessellation, TextureHandle texture);
        void Render();

    public:
        const auto &GetVertices() const
        {
            return m_Vertices;
        }

        static InputLayout m_Layout;

    private:
        TextureHandle m_Texture = 0;

        RenderBufferHandle m_VertexBuffer = 0;
        RenderBufferHandle m_IndexBuffer = 0;

        eastl::vector<MeshVertex> m_Vertices;
        u32 m_IndexCount = 0;
    };

}  // namespace lr