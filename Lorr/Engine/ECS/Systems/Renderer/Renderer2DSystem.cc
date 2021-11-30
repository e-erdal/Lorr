#include "Renderer2DSystem.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"
#include "Engine/Managers/ShaderManager.hh"

#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

namespace Lorr::System
{
    void Renderer2DSystem::Tick(float deltaTime)
    {
    }

    void Renderer2DSystem::Draw()
    {
        ZoneScoped;

        IRenderer *pRenderer = GetEngine()->GetRenderer();
        ShaderManager *pShaderMan = GetEngine()->GetShaderMan();

        // Stuff that renderer requires
        Camera2D *pCamera = GetEngine()->GetCamera2D();
        auto cameraMatrix = pCamera->GetMatrix();
        ShaderProgram *pBatcherProgram = pShaderMan->GetProgram("shader://batcher");
        RenderBufferHandle batcherCBuf = pShaderMan->GetRenderBuffer("cbuffer://batcher");
        ShaderProgram *pFontProgram = pShaderMan->GetProgram("shader://font");
        RenderBufferHandle fontCBuf = pShaderMan->GetRenderBuffer("cbuffer://font");

        // Renderer
        VertexBatcher *pBatcher = GetEngine()->GetBatcher();
        batcherCBuf->SetData(&cameraMatrix[0][0], sizeof(glm::mat4));

        pBatcher->SetCurrentProgram(pBatcherProgram);
        pBatcher->Begin();

        pRenderer->UseConstantBuffer(batcherCBuf, RenderBufferTarget::Vertex, 0);

        m_pRegistry->view<Component::Transform>().each([&](auto entity, Component::Transform &transform) {
            if (m_pRegistry->has<Component::Renderable>(entity))
            {
                auto &renderable = m_pRegistry->get<Component::Renderable>(entity);
                pBatcher->SetCurrentTexture(renderable.texture);
                pBatcher->PushRect(transform.Matrix, renderable.Color);
            }
        });

        pBatcher->Reset();
        pBatcher->SetCurrentProgram(pFontProgram);
        pRenderer->UseConstantBuffer(batcherCBuf, RenderBufferTarget::Vertex, 0);

        FontRenderBuffer fontRenderBufferData;

        m_pRegistry->view<Component::Transform>().each([&](auto entity, Component::Transform &transform) {
            if (m_pRegistry->has<Component::Text>(entity))
            {
                Component::Text &text = m_pRegistry->get<Component::Text>(entity);

                /// Calculate distance factor
                float distanceFactor = text.m_PixelRange * (text.m_Texture->GetWidth() / text.m_SizePx);

                fontRenderBufferData.RangePx = glm::vec4(distanceFactor, 0, 0, 0);
                fontCBuf->SetData(&fontRenderBufferData, sizeof(FontRenderBuffer));
                pRenderer->UseConstantBuffer(fontCBuf, RenderBufferTarget::Pixel, 0);

                pBatcher->SetCurrentTexture(text.m_Texture);

                glm::mat4 matrix = Math::CalcTransform(transform.Position, glm::vec3(transform.Size.x));

                for (const auto &c : text.m_Chars)
                {
                    auto pVertex = pBatcher->AllocVertex();

                    pVertex->Position = matrix * glm::vec4(c.Bounds.x, c.Bounds.y, 1, 1);
                    pVertex->TexCoord = glm::vec2(c.UV.x, c.UV.y);
                    pVertex->Color = glm::vec4(255);
                    pVertex++;

                    pVertex->Position = matrix * glm::vec4(c.Bounds.x, c.Bounds.w, 1, 1);
                    pVertex->TexCoord = glm::vec2(c.UV.x, c.UV.w);
                    pVertex->Color = glm::vec4(255);
                    pVertex++;

                    pVertex->Position = matrix * glm::vec4(c.Bounds.z, c.Bounds.w, 1, 1);
                    pVertex->TexCoord = glm::vec2(c.UV.z, c.UV.w);
                    pVertex->Color = glm::vec4(255);
                    pVertex++;

                    pVertex->Position = matrix * glm::vec4(c.Bounds.z, c.Bounds.y, 1, 1);
                    pVertex->TexCoord = glm::vec2(c.UV.z, c.UV.y);
                    pVertex->Color = glm::vec4(255);
                    pVertex++;
                }
            }
        });

        pBatcher->End();
    }

}  // namespace Lorr::System