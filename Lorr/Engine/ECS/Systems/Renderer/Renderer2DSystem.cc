#include "Renderer2DSystem.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Managers/ShaderManager.hh"

#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

namespace lr::System
{
    void Renderer2DSystem::Init()
    {
        ShaderManager *pShaderMan = GetEngine()->GetShaderMan();

        //* Shaders *//
        RenderBufferDesc genericDynBufferDesc;
        genericDynBufferDesc.Type = RenderBufferType::Constant;
        genericDynBufferDesc.Usage = RenderBufferUsage::Dynamic;
        genericDynBufferDesc.MemFlags = RenderBufferMemoryFlags::Access_CPUW;

        genericDynBufferDesc.DataLen = sizeof(Batcher2DBufferData);
        pShaderMan->CreateRenderBuffer("cbuffer://batcher2d", genericDynBufferDesc);
        pShaderMan->CreateProgram("shader://batcher", Renderer2D::m_Batcher2DLayout, "shader:batch.v", "shader:batch.p");

        genericDynBufferDesc.DataLen = sizeof(FontRenderBuffer);
        pShaderMan->CreateRenderBuffer("cbuffer://font", genericDynBufferDesc);
        pShaderMan->CreateProgram("shader://font", Renderer2D::m_Batcher2DLayout, "shader:font.v", "shader:font.p");
    }

    void Renderer2DSystem::Tick(float deltaTime)
    {
    }

    void Renderer2DSystem::Draw()
    {
        ZoneScoped;

        BaseRenderer *pRenderer = GetEngine()->GetRenderer();
        ShaderManager *pShaderMan = GetEngine()->GetShaderMan();
        auto pBatcher = GetEngine()->GetRenderer2D()->GetBatcher();

        // Stuff that renderer requires
        ShaderProgram *pBatcherProgram = pShaderMan->GetProgram("shader://batcher");
        ShaderProgram *pFontProgram = pShaderMan->GetProgram("shader://font");

        RenderBufferHandle batcherCBuf = pShaderMan->GetRenderBuffer("cbuffer://batcher2d");
        RenderBufferHandle fontCBuf = pShaderMan->GetRenderBuffer("cbuffer://font");

        Batcher2DBufferData batcherCBufData = { .Matrix = GetApp()->GetActiveScene()->GetEntity("entity://camera2d").GetCameraMatrix() };

        pRenderer->MapBuffer(batcherCBuf, &batcherCBufData, sizeof(Batcher2DBufferData));

        pBatcher->SetCurrentProgram(pBatcherProgram);
        pBatcher->Begin();

        pRenderer->SetConstantBuffer(batcherCBuf, RenderBufferTarget::Vertex, 0);

        m_pRegistry->view<Component::Transform>().each([&](auto entity, Component::Transform &transform) {
            /// Render a rectangle
            if (m_pRegistry->has<Component::Renderable>(entity))
            {
                auto &renderable = m_pRegistry->get<Component::Renderable>(entity);
                pBatcher->SetCurrentTexture(renderable.texture);
                pBatcher->PushRect(transform.Matrix, renderable.Color);
            }

            /// Render a text
            if (m_pRegistry->has<Component::Text>(entity))
            {
                /// We don't want to mix other draw events with this. Quite expensive but required
                pBatcher->End();

                auto &text = m_pRegistry->get<Component::Text>(entity);

                /// Calculate distance factor and set it to pixel buffer
                FontRenderBuffer fontRenderBufferData;
                float distanceFactor = text.m_PixelRange * (transform.Size.x / text.m_SizePx);
                fontRenderBufferData.RangePx = glm::vec4(distanceFactor, 0, 0, 0);

                pRenderer->MapBuffer(fontCBuf, &fontRenderBufferData, sizeof(FontRenderBuffer));

                pRenderer->SetConstantBuffer(fontCBuf, RenderBufferTarget::Pixel, 0);

                /// Set shader resources
                pBatcher->SetCurrentTexture(text.m_Texture);
                pBatcher->SetCurrentProgram(pFontProgram);

                /// Calculate screen coordinates matrix, this is not the actual matrix of each char
                glm::mat4 matrix = Math::CalcTransform(transform.Position, transform.Size);

                /// Process all lines depending on their alignment
                for (const auto &line : text.m_Lines)
                {
                    float verticalPos = 0;

                    switch (text.m_Alignment)
                    {
                        case TextAlignment::Left: break;
                        case TextAlignment::Right: verticalPos = text.m_Size.x - line.Width; break;
                        case TextAlignment::Middle: verticalPos = text.m_Size.x / 2 - line.Width / 2; break;
                    }

                    for (const auto &c : line.Chars)
                    {
                        // TODO: This is extremely expensive but penalty of using high quality fonts. So find a better way of doing this.
                        glm::mat4 charMat = glm::translate(matrix, glm::vec3(c.Position, 1) + glm::vec3(verticalPos, 0, 0));
                        charMat = glm::scale(charMat, glm::vec3(c.Size, 1));
                        pBatcher->PushRect(charMat, c.UV);
                    }
                }

                /// Processing is now done, draw everything in batch.
                /// We want "a draw call for a component" so constant buffer will be different for all text components
                pBatcher->End();
            }
        });

        pBatcher->End();
    }  // namespace lr::System

}  // namespace lr::System