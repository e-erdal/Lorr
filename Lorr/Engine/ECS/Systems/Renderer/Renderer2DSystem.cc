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

        // Stuff that renderer requires
        Camera2D *pCamera = GetEngine()->GetCamera2D();
        auto cameraMatrix = pCamera->GetMatrix();
        ShaderProgram *pBatcherProgram = GetEngine()->GetShaderMan()->GetProgram("shader://batcher");
        RenderBufferHandle batcherCBuf = GetEngine()->GetShaderMan()->GetRenderBuffer("cbuffer://batcher");

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

        pBatcher->End();
    }

}  // namespace Lorr::System