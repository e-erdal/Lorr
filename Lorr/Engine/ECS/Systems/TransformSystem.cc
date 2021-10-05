#include "TransformSystem.hh"

#include "Engine/ECS/Components/TransformComponent.hh"

namespace Lorr::System
{
    void TransformSystem::Tick(float deltaTime)
    {
        auto group = m_pRegistry->view<Component::Transform>();

        for (auto entity : group)
        {
            auto &transform = group.get<Component::Transform>(entity);

            transform.Matrix = glm::mat4{ 1.0f };
            transform.Matrix = glm::translate(transform.Matrix, transform.Position);
            transform.Matrix = glm::rotate(transform.Matrix, glm::radians(transform.Rotation), glm::vec3(0.f, 0.f, 1.f));
            transform.Matrix = glm::scale(transform.Matrix, transform.Size);
        }
    }

    void TransformSystem::Draw()
    {
    }

}  // namespace Lorr::System