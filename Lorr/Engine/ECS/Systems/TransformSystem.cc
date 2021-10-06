#include "TransformSystem.hh"

#include "Engine/ECS/Components/TransformComponent.hh"

namespace Lorr::System
{
    void TransformSystem::Tick(float deltaTime)
    {
        glm::mat4 matrix(1.0f);
        auto view = m_pRegistry->view<Component::Transform>();

        for (auto entity : view)
        {
            auto &transform = view.get<Component::Transform>(entity);

            Math::SetPos(matrix, transform.Position);
            matrix = glm::rotate(matrix, glm::radians(transform.Rotation), glm::vec3(0.f, 0.f, 1.f));
            Math::SetSize(matrix, transform.Size);
            transform.Matrix = matrix;
        }
    }

    void TransformSystem::Draw()
    {
    }

}  // namespace Lorr::System