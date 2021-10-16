#include "RendererSystem.hh"

#include "Engine/App/Engine.hh"

#include "Engine/Managers/ShaderManager.hh"

#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

#include "Engine/Model/Model.hh"

namespace Lorr::System
{
    void RendererSystem::Tick(float deltaTime)
    {
    }

    void RendererSystem::Draw()
    {
        glm::mat4 matrix(1.0f);

        // Stuff that renderer requires
        Camera3D *pCamera = GetEngine()->GetCamera();
        glm::mat4 cameraMatrix = glm::transpose(pCamera->GetProjection() * pCamera->GetView());

        // Model shader
        ShaderProgram *modelProgram = GetEngine()->GetShaderMan()->Get("game://model");
        RenderBufferHandle modelCBuf = GetEngine()->GetShaderMan()->GetCBuf("game://model");
        modelCBuf->SetData(&cameraMatrix[0][0], sizeof(glm::mat4));

        // Renderer
        VertexBatcher *pBatcher = GetEngine()->GetBatcher();
        pBatcher->Begin();

        m_pRegistry->view<Component::Transform>().each([&](auto entity, Component::Transform &transform) {
            if (m_pRegistry->has<Component::Renderable>(entity))
            {
                auto &renderable = m_pRegistry->get<Component::Renderable>(entity);
                if (renderable.IsBatch) pBatcher->PushRect(renderable.texture, transform.Matrix, renderable.Color);
            }

            if (m_pRegistry->has<Component::Text>(entity))
            {
                auto &text = m_pRegistry->get<Component::Text>(entity);
                for (const auto &c : text.Chars)
                {
                    Math::SetPos(matrix, glm::vec3(transform.Position.x + c.Pos.x, transform.Position.y + c.Pos.y, transform.Position.z));
                    Math::SetSize(matrix, glm::vec3(c.Size.x, c.Size.y, 1));
                    pBatcher->PushRect(text.texture, matrix, c.UV, { 255, 255, 255, 255 });
                }
            }

            if (m_pRegistry->has<Model>(entity))
            {
                Model &model = m_pRegistry->get<Model>(entity);

                modelProgram->Vertex->Use();
                modelProgram->Pixel->Use();
                modelCBuf->Use(0);

                model.GetVertexBuf()->Use(0, &kMeshLayout);
                model.GetIndexBuf()->Use(0);

                GetEngine()->GetRenderer()->Draw(model.GetIndexCount());
            }
        });

        pBatcher->End();
    }

}  // namespace Lorr::System