#include "RendererSystem.hh"

#include "Engine/App/Engine.hh"

#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/RenderableComponent.hh"

namespace Lorr::System
{
    void RendererSystem::Tick(float deltaTime)
    {
    }

    void RendererSystem::Draw()
    {
        VertexBatcher *pBatcher = GetEngine()->GetBatcher();
        pBatcher->Begin();

        auto group = m_pRegistry->view<Component::Transform, Component::Renderable>();
        for (auto entity : group)
        {
            auto &&[transform, renderable] = group.get<Component::Transform, Component::Renderable>(entity);

            if (renderable.IsBatch) pBatcher->PushRect(renderable.pTexture, transform.Matrix, renderable.TextureCoords, renderable.Color);
        }

        pBatcher->End();
    }

}  // namespace Lorr::System