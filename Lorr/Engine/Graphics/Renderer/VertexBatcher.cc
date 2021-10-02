#include "VertexBatcher.hh"

#include "Engine/App/Engine.hh"

namespace Lorr
{
    constexpr uint32_t kMaxRects = 5000;

    void VertexBatcher::Init()
    {
        m_Layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float, true)
            .end();

        uint32_t *quads = new uint32_t[kMaxRects * 6];
        uint32_t offset = 0;

        for (uint32_t i = 0; i < (kMaxRects * 6); i += 6)
        {
            quads[i + 0] = offset + 0;
            quads[i + 1] = offset + 1;
            quads[i + 2] = offset + 2;

            quads[i + 3] = offset + 2;
            quads[i + 4] = offset + 3;
            quads[i + 5] = offset + 0;

            offset += 4;
        }

        m_IndexBuffer = bgfx::createIndexBuffer(bgfx::copy(quads, kMaxRects * 6 * sizeof(uint32_t)), BGFX_BUFFER_INDEX32);
        delete[] quads;

        m_TextureUniform = bgfx::createUniform("s_Texture", bgfx::UniformType::Sampler);

        Shader *pShader = GetEngine()->GetResourceMan()->LoadShader("engine://batcher-program", "batch.lr", false);
        m_ShaderProgram = pShader->GetHandle();
    }

    void VertexBatcher::Begin()
    {
    }

    void VertexBatcher::End()
    {
        Reset();
    }

    void VertexBatcher::Flush()
    {
        for (auto &&event : m_Queue)
        {
            Texture2D *pTexture = event.first;
            auto &vertexes = event.second.Vertices;
            auto &indexes = event.second.Indexes;

            if (vertexes.size() > 0)
            {
                bgfx::TransientVertexBuffer tvb;
                bgfx::allocTransientVertexBuffer(&tvb, vertexes.size(), m_Layout);
                memcpy(tvb.data, &vertexes[0], vertexes.size() * sizeof(BatcherVertex));
                bgfx::setVertexBuffer(0, &tvb, 0, vertexes.size());

                // This is wrong, nvm, not for now
                uint32_t count = indexes ? indexes : kMaxRects * 6;
                bgfx::setIndexBuffer(m_IndexBuffer, 0, count);

                GetEngine()->GetRenderer()->SetTexture(pTexture, m_TextureUniform);
                bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_BLEND_ALPHA);
                bgfx::submit(0, m_ShaderProgram);
            }
        }
    }

    void VertexBatcher::Reset()
    {
        Flush();
        m_Queue.clear();
    }

    void VertexBatcher::PushRect(Texture2D *pTexture, const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color)
    {
        BatcherEvent &event = GetEvent(!pTexture ? GetEngine()->GetRenderer()->GetPlaceholder() : pTexture);
        BatcherVertices &vertices = event.Vertices;
        vertices.resize(vertices.size() + 4);

        BatcherVertex *info = &vertices[vertices.size() - 4];

        for (size_t i = 0; i < 4; i++)
        {
            info->Pos = transform * kVertexPos[i];
            info->UV = kVertexUV[i];
            info->Color = (glm::vec4)color / 255.f;
            info++;
        }

        event.Indexes += 6;
    }

    BatcherEvent &VertexBatcher::GetEvent(Texture2D *pTexture)
    {
        constexpr uint32_t allocSize = 128 * 4 * sizeof(BatcherVertex);

        if (m_Queue.size() > 0 && m_Queue.back().first == pTexture) return m_Queue.back().second;

        m_Queue.push_back(std::make_pair(pTexture, BatcherEvent{}));
        m_Queue.back().second.Vertices.reserve(allocSize);

        return m_Queue.back().second;
    }

}  // namespace Lorr