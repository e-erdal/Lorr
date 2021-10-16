#include "VertexBatcher.hh"

#include "Engine/App/Engine.hh"

namespace Lorr
{

    BatcherVertex cubeVertices[] = {
        { { -1.0, 1.0, -1.0 }, { 0.0, 0.0 }, { 0.0, 0.0, -1.0, -1.0 } },  { { 1.0, 1.0, -1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, -1.0, -1.0 } },
        { { -1.0, -1.0, -1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, -1.0, -1.0 } }, { { -1.0, -1.0, -1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, -1.0, -1.0 } },
        { { 1.0, 1.0, -1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, -1.0, -1.0 } },   { { 1.0, -1.0, -1.0 }, { 1.0, 1.0 }, { 0.0, 0.0, -1.0, -1.0 } },
        { { 1.0, 1.0, -1.0 }, { 0.0, 0.0 }, { 1.0, 0.0, 0.0, -1.0 } },    { { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 1.0, 0.0, 0.0, -1.0 } },
        { { 1.0, -1.0, -1.0 }, { 0.0, 1.0 }, { 1.0, 0.0, 0.0, -1.0 } },   { { 1.0, -1.0, -1.0 }, { 0.0, 1.0 }, { 1.0, 0.0, 0.0, -1.0 } },
        { { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 1.0, 0.0, 0.0, -1.0 } },     { { 1.0, -1.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0, 0.0, -1.0 } },
        { { 1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { 0.0, 0.0, 1.0, -1.0 } },     { { -1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, 1.0, -1.0 } },
        { { 1.0, -1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, 1.0, -1.0 } },    { { 1.0, -1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, 1.0, -1.0 } },
        { { -1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, 1.0, -1.0 } },    { { -1.0, -1.0, 1.0 }, { 1.0, 1.0 }, { 0.0, 0.0, 1.0, -1.0 } },
        { { -1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { -1.0, 0.0, 0.0, -1.0 } },   { { -1.0, 1.0, -1.0 }, { 1.0, 0.0 }, { -1.0, 0.0, 0.0, -1.0 } },
        { { -1.0, -1.0, 1.0 }, { 0.0, 1.0 }, { -1.0, 0.0, 0.0, -1.0 } },  { { -1.0, -1.0, 1.0 }, { 0.0, 1.0 }, { -1.0, 0.0, 0.0, -1.0 } },
        { { -1.0, 1.0, -1.0 }, { 1.0, 0.0 }, { -1.0, 0.0, 0.0, -1.0 } },  { { -1.0, -1.0, -1.0 }, { 1.0, 1.0 }, { -1.0, 0.0, 0.0, -1.0 } },
        { { -1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { 0.0, 1.0, 0.0, -1.0 } },    { { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 1.0, 0.0, -1.0 } },
        { { -1.0, 1.0, -1.0 }, { 0.0, 1.0 }, { 0.0, 1.0, 0.0, -1.0 } },   { { -1.0, 1.0, -1.0 }, { 0.0, 1.0 }, { 0.0, 1.0, 0.0, -1.0 } },
        { { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 1.0, 0.0, -1.0 } },     { { 1.0, 1.0, -1.0 }, { 1.0, 1.0 }, { 0.0, 1.0, 0.0, -1.0 } },
        { { -1.0, -1.0, -1.0 }, { 0.0, 0.0 }, { 0.0, -1.0, 0.0, -1.0 } }, { { 1.0, -1.0, -1.0 }, { 1.0, 0.0 }, { 0.0, -1.0, 0.0, -1.0 } },
        { { -1.0, -1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, -1.0, 0.0, -1.0 } },  { { -1.0, -1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, -1.0, 0.0, -1.0 } },
        { { 1.0, -1.0, -1.0 }, { 1.0, 0.0 }, { 0.0, -1.0, 0.0, -1.0 } },  { { 1.0, -1.0, 1.0 }, { 1.0, 1.0 }, { 0.0, -1.0, 0.0, -1.0 } },
    };

    constexpr uint32_t kMaxRects = _countof(cubeVertices) / 4;
    constexpr uint32_t kTransientBufSize = kMaxRects * sizeof(BatcherVertex) * 4;

    void VertexBatcher::Init()
    {
        m_Layout = {
            { VertexAttribType::Vec3, "POSITION" },
            { VertexAttribType::Vec2, "TEXCOORD" },
            { VertexAttribType::Vec4, "COLOR" },
        };

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

        // TODO: Create a buffer manager, this will also help to keep track on buffer lifetime and types
        void *pTempData = malloc(kTransientBufSize);
        memset(pTempData, 0, kTransientBufSize);

        m_VertexBuffer =
            RenderBuffer::Create(pTempData, kTransientBufSize, RenderBufferType::Vertex, RenderBufferUsage::Dynamic, RB_ACCESS_TYPE_CPUW);
        free(pTempData);

        m_IndexBuffer = RenderBuffer::Create(quads, kMaxRects * 6 * sizeof(uint32_t), RenderBufferType::Index);
        delete[] quads;

        m_ConstantBuffer =
            RenderBuffer::Create(0, sizeof(glm::mat4), RenderBufferType::Constant, RenderBufferUsage::Dynamic, RB_ACCESS_TYPE_CPUW);

        ShaderDesc vertDesc;
        vertDesc.Layout = m_Layout;
        m_VertexShader = Shader::Create("engine://batcher-program-vert", "batchv.lr", &vertDesc);
        m_PixelShader = Shader::Create("engine://batcher-program-pixel", "batchp.lr");
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
        Renderer *pRenderer = GetEngine()->GetRenderer();
        auto *pCamera = GetEngine()->GetCamera();
        glm::mat4 mvp = glm::transpose(pCamera->GetProjection() * pCamera->GetView());

        for (auto &&event : m_Queue)
        {
            TextureHandle texture = event.first;
            auto &vertexes = event.second.Vertices;
            auto &indexes = event.second.Indexes;

            if (vertexes.size() > 0)
            {
                m_ConstantBuffer->SetData(&mvp[0][0], sizeof(mvp));
                m_ConstantBuffer->Use(0);

                m_VertexShader->Use();
                m_PixelShader->Use();

                m_VertexBuffer->SetData(cubeVertices, sizeof(cubeVertices));
                m_VertexBuffer->Use(0, &m_Layout);

                m_IndexBuffer->Use(0);

                uint32_t indexCount = indexes ? indexes : kMaxRects * 6;
                pRenderer->Draw(54);
            }
        }
    }

    void VertexBatcher::Reset()
    {
        Flush();
        m_Queue.clear();
    }

    void VertexBatcher::PushRect(TextureHandle texture, const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color)
    {
        const glm::mat4x2 uvMat = { uv.z, uv.w, uv.z, uv.y, uv.x, uv.y, uv.x, uv.w };
        PushRect(texture, glm::transpose(transform), uvMat, color);
    }

    void VertexBatcher::PushRect(TextureHandle texture, const glm::mat4 &transform, const glm::mat4x2 &uv, const glm::ivec4 &color)
    {
        BatcherEvent &event = GetEvent(!texture ? GetEngine()->GetRenderer()->GetPlaceholder() : texture);
        BatcherVertices &vertices = event.Vertices;
        vertices.resize(vertices.size() + 4);

        BatcherVertex *info = &vertices[vertices.size() - 4];

        for (size_t i = 0; i < 4; i++)
        {
            info->Pos = transform * kVertexPos[i];
            info->UV = uv[i];
            info->Color = (glm::vec4)color / 255.f;
            info++;
        }

        event.Indexes += 6;
    }

    void VertexBatcher::PushRect(TextureHandle texture, const glm::mat4 &transform, const glm::ivec4 &color)
    {
        PushRect(texture, transform, kVertexUV, color);
    }

    BatcherEvent &VertexBatcher::GetEvent(TextureHandle texture)
    {
        constexpr uint32_t allocSize = 128 * 4 * sizeof(BatcherVertex);

        if (m_Queue.size() > 0 && m_Queue.back().first == texture) return m_Queue.back().second;

        m_Queue.push_back(std::make_pair(texture, BatcherEvent{}));
        m_Queue.back().second.Vertices.reserve(allocSize);

        return m_Queue.back().second;
    }

}  // namespace Lorr