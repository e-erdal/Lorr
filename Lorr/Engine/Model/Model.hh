//
// Created on Saturday 16th October 2021 by e-erdal
//

#pragma once

#include "Mesh.hh"

#include "Engine/Graphics/Common/IRenderBuffer.hh"

namespace Lorr
{
    class Model
    {
    public:
        Model() = default;

        void Init(const std::string &path);

    public:
        const RenderBufferHandle &GetVertexBuf() const
        {
            return m_VertexBuffer;
        }

        const RenderBufferHandle &GetIndexBuf() const
        {
            return m_IndexBuffer;
        }

        uint32_t GetIndexCount()
        {
            return m_IndexCount;
        }

    private:
        std::vector<Mesh> m_Meshes;

        RenderBufferHandle m_VertexBuffer = 0;
        RenderBufferHandle m_IndexBuffer = 0;

        uint32_t m_IndexCount = 0;
    };

}  // namespace Lorr