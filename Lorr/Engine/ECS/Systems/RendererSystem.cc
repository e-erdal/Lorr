#include "RendererSystem.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/RenderableComponent.hh"
#include "Engine/ECS/Components/TextComponent.hh"

namespace Lorr::System
{
    void RendererSystem::Tick(float deltaTime)
    {
    }

    void RendererSystem::Draw()
    {
        glm::mat4 matrix(1.0f);

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
        });

        pBatcher->End();
    }

}  // namespace Lorr::System