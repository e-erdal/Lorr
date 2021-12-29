//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/Common/BaseRenderer.hh"
#include "Engine/Graphics/Common/BaseTexture.hh"
#include "Engine/Graphics/Common/BaseShader.hh"
#include "Engine/Graphics/Common/BaseRenderBuffer.hh"

#include "Engine/Managers/ShaderManager.hh"

namespace lr
{
    template<typename VertexT, u32 VertexCount>
    class VertexBatcher
    {
    public:
        VertexBatcher() = default;

        void Init(const InputLayout &inputLayout)
        {
            ZoneScoped;

            if (!m_IndexBuffer)
            {
                u32 *pQuads = new u32[VertexCount * 6];
                u32 offset = 0;

                for (u32 i = 0; i < (VertexCount * 6); i += 6)
                {
                    pQuads[i + 0] = offset + 0;
                    pQuads[i + 1] = offset + 1;
                    pQuads[i + 2] = offset + 2;

                    pQuads[i + 3] = offset + 2;
                    pQuads[i + 4] = offset + 3;
                    pQuads[i + 5] = offset + 0;

                    offset += 4;
                }

                RenderBufferDesc desc;
                desc.pData = pQuads;
                desc.DataLen = VertexCount * 6 * sizeof(u32);
                desc.Type = RenderBufferType::Index;

                m_IndexBuffer = RenderBuffer::Create(desc);

                delete[] pQuads;

                desc.pData = nullptr;
                desc.DataLen = VertexCount * sizeof(VertexT);
                desc.Type = RenderBufferType::Vertex;
                desc.Usage = RenderBufferUsage::Dynamic;
                desc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

                m_VertexBuffer = RenderBuffer::Create(desc);
            }

            m_InputLayout = inputLayout;
        }

        void Init(const InputLayout &inputLayout, RenderBufferHandle indexBuffer)
        {
            ZoneScoped;

            m_IndexBuffer = indexBuffer;

            Init();
        }

        void Begin()
        {
            ZoneScoped;
        }

        void End(bool discard = true)
        {
            ZoneScoped;

            BaseRenderer *pRenderer = GetEngine()->GetRenderer();

            if (m_VerticesOffset > 0 && m_CurrentTexture)
            {
                pRenderer->UseShader(m_pShaderProgram->Vertex);
                pRenderer->UseShader(m_pShaderProgram->Pixel);

                pRenderer->MapBuffer(m_VertexBuffer, &m_Vertices[0], m_Vertices.size() * sizeof(VertexT));
                pRenderer->UnmapBuffer(m_VertexBuffer);

                pRenderer->UseVertexBuffer(m_VertexBuffer, &m_InputLayout);
                pRenderer->UseIndexBuffer(m_IndexBuffer);

                m_CurrentTexture->Use();
                pRenderer->DrawIndexed(m_IndexCount);
            }

            if (discard)
            {
                DiscardBuffer();
            }
        }

        void SetCurrentTexture(TextureHandle texture)
        {
            ZoneScoped;

            if (texture != m_CurrentTexture)
            {
                End();
                m_CurrentTexture = texture;
            }
        }

        void SetCurrentProgram(ShaderProgram *pProgram)
        {
            ZoneScoped;

            if (pProgram != m_pShaderProgram)
            {
                End();
                m_pShaderProgram = pProgram;
            }
        }

        void PushRect(const glm::mat4 &transform, const glm::ivec4 &color = { 255, 255, 255, 255 })
        {
            ZoneScoped;

            PushRect(transform, glm::mat4(1.0f), color);
        }

        void PushRect(const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color = { 255, 255, 255, 255 })
        {
            ZoneScoped;

            const glm::mat4x2 uvMat = { uv.z, uv.w, uv.z, uv.y, uv.x, uv.y, uv.x, uv.w };
            PushRect(transform, uvMat, color);
        }

        void PushRect(const glm::mat4 &transform, const glm::mat4x2 &uv, const glm::ivec4 &color = { 255, 255, 255, 255 })
        {
            ZoneScoped;

            VertexT *pVertices = AllocRect(1);

            for (u32 i = 0; i < 4; i++)
            {
                VertexT::CalcFn(pVertices, i, transform, uv, color);
                pVertices++;
            }
        }

        void SetRects(VertexT *pVertices, u32 count)
        {
            ZoneScoped;

            VertexT *pVerticesPtr = pVertices;
            while (count != 0)
            {
                u32 allocatedCount = 0;
                VertexT *pAlloc = AllocRect(count, &allocatedCount);
                memcpy(pAlloc, pVerticesPtr, 4 * allocatedCount * sizeof(VertexT));

                if (m_VerticesOffset == VertexCount)
                {
                    End();
                }

                pVerticesPtr += 4 * allocatedCount;
                count -= allocatedCount;
            }
        }

        VertexT *AllocRect(u32 count, u32 *pOutAllocated = nullptr)
        {
            ZoneScoped;

            u32 diff = VertexCount - m_VerticesOffset;
            count = std::min(diff / 4, count);
            if (pOutAllocated) *pOutAllocated = count;

            VertexT *pVertices = &m_Vertices[m_VerticesOffset];
            m_VerticesOffset += count * 4;
            m_IndexCount += count * 6;

            return pVertices;
        }

        void DiscardBuffer()
        {
            m_VerticesOffset = 0;
            m_IndexCount = 0;
        }

    private:
        eastl::array<VertexT, VertexCount> m_Vertices;
        u32 m_VerticesOffset = 0;
        u32 m_IndexCount = 0;

        InputLayout m_InputLayout;
        RenderBufferHandle m_VertexBuffer = nullptr;
        RenderBufferHandle m_IndexBuffer = nullptr;

        ShaderProgram *m_pShaderProgram = 0;
        TextureHandle m_CurrentTexture = 0;
    };

}  // namespace lr