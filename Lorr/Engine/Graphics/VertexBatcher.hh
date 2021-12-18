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

        void End()
        {
            ZoneScoped;

            BaseRenderer *pRenderer = GetEngine()->GetRenderer();

            SAFE_DELETE(m_VertexBuffer);

            if (m_VerticesOffset > 0 && m_CurrentTexture)
            {
                pRenderer->UseShader(m_pShaderProgram->Vertex);
                pRenderer->UseShader(m_pShaderProgram->Pixel);

                RenderBufferDesc desc;
                desc.pData = m_Vertices.data();
                desc.DataLen = m_VerticesOffset * sizeof(VertexT);
                desc.Type = RenderBufferType::Vertex;
                desc.Usage = RenderBufferUsage::Dynamic;
                desc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

                m_VertexBuffer = RenderBuffer::Create(desc);

                pRenderer->UseVertexBuffer(m_VertexBuffer, &m_InputLayout);
                pRenderer->UseIndexBuffer(m_IndexBuffer);

                m_CurrentTexture->Use();
                pRenderer->DrawIndexed(m_IndexCount);
            }

            m_VerticesOffset = 0;
            m_IndexCount = 0;
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
            PushRect(transform, glm::mat4(1.0f), color);
        }

        void PushRect(const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color = { 255, 255, 255, 255 })
        {
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

        VertexT *AllocRect(u32 count)
        {
            ZoneScoped;

            VertexT *pVertices = &m_Vertices[m_VerticesOffset];
            m_VerticesOffset += count * 4;
            m_IndexCount += count * 6;

            return pVertices;
        }

    private:
        std::array<VertexT, VertexCount> m_Vertices;
        u32 m_VerticesOffset = 0;
        u32 m_IndexCount = 0;

        InputLayout m_InputLayout;
        RenderBufferHandle m_VertexBuffer = nullptr;
        RenderBufferHandle m_IndexBuffer = nullptr;

        ShaderProgram *m_pShaderProgram = 0;
        TextureHandle m_CurrentTexture = 0;
    };

}  // namespace lr