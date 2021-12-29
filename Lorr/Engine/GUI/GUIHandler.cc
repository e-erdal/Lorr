#include "GUIHandler.hh"

#include "Engine/Graphics/VertexBatcher.hh"

namespace lr
{
    constexpr glm::mat4 kGUIVertexRectPos = {
        1, 1, 1, 1,  // V1
        1, 0, 1, 1,  // V2
        0, 0, 1, 1,  // V3
        0, 1, 1, 1   // V4
    };

    constexpr glm::mat4x2 kGUIVertexRectUV = {
        1, 1,  // V1
        1, 0,  // V2
        0, 0,  // V3
        0, 1,  // V4
    };

    static InputLayout kGUIInputLayout = {
        { VertexAttribType::Vec2, "POSITION" },
        { VertexAttribType::Vec2, "TEXCOORD" },
        { VertexAttribType::UInt, "COLOR" },
    };

    static GUI::Context *s_pContext = nullptr;

    /// INTERNAL ///

    GUI::Panel *CreatePanel()
    {
        s_pContext->m_Panels.push_back(GUI::Panel{});
        return &s_pContext->m_Panels.back();
    }

    /// Panel ///
    GUI::DrawData *GUI::Panel::GetDrawData(TextureHandle texture)
    {
        if (!texture) texture = GetEngine()->GetRenderer()->GetPlaceholder();

        auto dataIt = s_pContext->m_DrawData.find(texture);
        if (dataIt == s_pContext->m_DrawData.end())
        {
            auto pairIt = s_pContext->m_DrawData.emplace(texture, DrawData{});
            return &pairIt.first->second;
        }

        return &dataIt->second;
    }

    void GUI::Panel::AddRect(const glm::vec2 &pos, const glm::vec2 &size, u32 color, TextureHandle texture)
    {
        DrawData *pDrawData = GetDrawData(texture);
        s_pContext->AllocVertices(4);

        glm::mat4 matrix = glm::translate(glm::mat4(1.0), glm::vec3(pos, 1)) * glm::scale(glm::mat4(1.0), glm::vec3(size, 1));
        for (size_t i = 0; i < 4; i++)
        {
            s_pContext->m_pVertPtr->Pos = matrix * kGUIVertexRectPos[i];
            s_pContext->m_pVertPtr->UV = kGUIVertexRectUV[i];
            s_pContext->m_pVertPtr->Color = color;
            s_pContext->m_pVertPtr++;
        }

        pDrawData->VertexCount += 4;
        pDrawData->IndexCount += 6;
    }

    /// Context ///
    void GUI::Context::AllocVertices(u32 size)
    {
        m_Vertices.resize(m_Vertices.size() + size);
        m_pVertPtr = &m_Vertices[m_Vertices.size() - size];
    }

    /// EXTERNAL ///
    void GUI::Init()
    {
        if (s_pContext)
        {
            LOG_ERROR("GUI module is already initialized.");
            return;
        }

        s_pContext = new GUI::Context;

        constexpr u16 kGUIIndexCount = 5000 * 6;
        u16 *pIndexes = new u16[kGUIIndexCount];

        u16 offset = 0;
        for (u16 i = 0; i < kGUIIndexCount; i += 6)
        {
            pIndexes[i + 0] = offset + 0;
            pIndexes[i + 1] = offset + 1;
            pIndexes[i + 2] = offset + 2;

            pIndexes[i + 3] = offset + 2;
            pIndexes[i + 4] = offset + 3;
            pIndexes[i + 5] = offset + 0;

            offset += 4;
        }

        RenderBufferDesc desc;
        desc.pData = pIndexes;
        desc.DataLen = kGUIIndexCount * sizeof(u32);
        desc.Type = RenderBufferType::Index;

        s_pContext->m_IndexBuffer = RenderBuffer::Create(desc);

        delete[] pIndexes;

        desc = {};
        desc.pData = 0;
        desc.DataLen = sizeof(GUI::ConstantBuffer);
        desc.Type = RenderBufferType::Constant;
        desc.Usage = RenderBufferUsage::Dynamic;
        desc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

        s_pContext->m_ConstantBuffer = RenderBuffer::Create(desc);
    }

    bool GUI::BeginPanel(const Identifier &ident, const glm::ivec2 &pos, const glm::ivec2 &size, const eastl::string &title)
    {
        s_pContext->m_pCurrentPanel = CreatePanel();

        s_pContext->m_pCurrentPanel->m_Pos = glm::vec3(pos, 999);
        s_pContext->m_pCurrentPanel->m_Size = size;

        return true;
    }

    void GUI::EndPanel()
    {
        s_pContext->m_pCurrentPanel = nullptr;
    }

    void GUI::BeginFrame()
    {
        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        /// We probably want to render this into backbuffer
        pRenderer->SetCurrentTarget("renderer://backbuffer");

        /// Set renderer states, no backup unlike imgui
        pRenderer->SetDepthFunc(D3D::DepthFunc::Always, false);
        pRenderer->SetCulling(D3D::Cull::None, false);
        pRenderer->SetBlend(true, false);

        /// Set render buffers
        pRenderer->UseShader(s_pContext->m_pBaseShaderProgram->Vertex);
        pRenderer->UseShader(s_pContext->m_pBaseShaderProgram->Pixel);

        GUI::ConstantBuffer constantBuffer;
        constantBuffer.Matrix = GetApp()->GetActiveScene()->GetEntity("entity://camera2d").GetCameraMatrix();
        constantBuffer.EdgeRadius = 0.f;
        // s_pContext->m_ConstantBuffer->SetData(&constantBuffer, sizeof(GUI::ConstantBuffer));

        /// Create transient vertex buffer
        SAFE_DELETE(s_pContext->m_VertexBuffer);
        RenderBufferDesc desc;
        desc.pData = &s_pContext->m_Vertices[0];
        desc.DataLen = s_pContext->m_Vertices.size() * sizeof(GUI::Vertex);
        desc.Type = RenderBufferType::Vertex;

        s_pContext->m_VertexBuffer = RenderBuffer::Create(desc);

        pRenderer->UseVertexBuffer(s_pContext->m_VertexBuffer, &kGUIInputLayout);
        pRenderer->UseIndexBuffer(s_pContext->m_IndexBuffer, false);
        pRenderer->UseShaderBuffer(s_pContext->m_ConstantBuffer, RenderBufferTarget::Vertex | RenderBufferTarget::Pixel, 0);
    }

    void GUI::Render()
    {
        BaseRenderer *pRenderer = GetEngine()->GetRenderer();

        u32 vertexOffset = 0;
        u32 indexOffset = 0;

        for (auto &call : s_pContext->m_DrawData)
        {
            auto &texture = call.first;
            auto &drawData = call.second;

            vertexOffset += drawData.VertexCount;
            indexOffset += drawData.IndexCount;

            texture->Use();
            pRenderer->DrawIndexed(drawData.IndexCount, indexOffset, vertexOffset);
        }
    }

    void GUI::EndFrame()
    {
    }

}  // namespace lr