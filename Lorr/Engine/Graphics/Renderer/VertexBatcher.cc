#include "VertexBatcher.hh"

#include "Engine/App/Engine.hh"

namespace Lorr
{
    constexpr u32 kMaxRects = 4000;
    constexpr u32 kTransientBufSize = kMaxRects * sizeof(BatcherVertex) * 4;

    InputLayout VertexBatcher::m_Layout;

    void VertexBatcher::Init()
    {
        ZoneScoped;
        
        m_Layout = {
            { VertexAttribType::Vec3, "POSITION" },
            { VertexAttribType::Vec2, "TEXCOORD" },
            { VertexAttribType::Vec4, "COLOR" },
        };

        u32 *quads = new u32[kMaxRects * 6];
        u32 offset = 0;

        for (u32 i = 0; i < (kMaxRects * 6); i += 6)
        {
            quads[i + 0] = offset + 0;
            quads[i + 1] = offset + 1;
            quads[i + 2] = offset + 2;

            quads[i + 3] = offset + 2;
            quads[i + 4] = offset + 3;
            quads[i + 5] = offset + 0;

            offset += 4;
        }

        RenderBufferDesc desc;
        desc.pData = quads;
        desc.DataLen = kMaxRects * 6 * sizeof(u32);
        desc.Type = RenderBufferType::Index;

        m_IndexBuffer = RenderBuffer::Create(desc);

        delete[] quads;
    }

    void VertexBatcher::Begin()
    {
    }

    void VertexBatcher::End()
    {
        ZoneScoped;
        
        Reset();
    }

    void VertexBatcher::Flush()
    {
        ZoneScoped;
        
        IRenderer *pRenderer = GetEngine()->GetRenderer();

        SAFE_DELETE(m_VertexBuffer);

        if (m_Vertices.size() > 0 && m_CurrentTexture)
        {
            pRenderer->UseShader(m_pShaderProgram->Vertex);
            pRenderer->UseShader(m_pShaderProgram->Pixel);

            RenderBufferDesc desc;
            desc.pData = &m_Vertices[0];
            desc.DataLen = m_Vertices.size() * sizeof(BatcherVertex);
            desc.Type = RenderBufferType::Vertex;
            desc.Usage = RenderBufferUsage::Dynamic;
            desc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

            m_VertexBuffer = RenderBuffer::Create(desc);

            pRenderer->UseVertexBuffer(m_VertexBuffer, &m_Layout);
            pRenderer->UseIndexBuffer(m_IndexBuffer);

            m_CurrentTexture->Use();
            pRenderer->DrawIndexed(m_Indexes);
        }
    }

    void VertexBatcher::Reset()
    {
        ZoneScoped;
        
        Flush();
        m_Vertices.clear();
        m_Indexes = 0;
    }

    void VertexBatcher::SetCurrentTexture(TextureHandle texture)
    {
        ZoneScoped;
        
        if (texture != m_CurrentTexture)
        {
            Reset();
            m_CurrentTexture = texture;
        }
    }

    void VertexBatcher::SetCurrentProgram(ShaderProgram *pProgram)
    {
        ZoneScoped;
        
        if (pProgram != m_pShaderProgram)
        {
            Reset();
            m_pShaderProgram = pProgram;
        }
    }

    void VertexBatcher::PushRect(const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color)
    {
        ZoneScoped;
        
        const glm::mat4x2 uvMat = { uv.z, uv.w, uv.z, uv.y, uv.x, uv.y, uv.x, uv.w };
        PushRect(glm::transpose(transform), uvMat, color);
    }

    void VertexBatcher::PushRect(const glm::mat4 &transform, const glm::mat4x2 &uv, const glm::ivec4 &color)
    {
        ZoneScoped;
        
        m_Vertices.resize(m_Vertices.size() + 4);

        BatcherVertex *info = &m_Vertices[m_Vertices.size() - 4];

        for (size_t i = 0; i < 4; i++)
        {
            info->Position = transform * kVertexPos[i];
            info->TexCoord = uv[i];
            info->Color = (glm::vec4)color / 255.f;
            info++;
        }

        m_Indexes += 6;
    }

    void VertexBatcher::PushRect(const glm::mat4 &transform, const glm::ivec4 &color)
    {
        ZoneScoped;
        
        PushRect(transform, kVertexUV, color);
    }

}  // namespace Lorr