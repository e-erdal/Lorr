#include "TransformSystem.hh"

#include "Engine/ECS/Components/TransformComponent.hh"

namespace Lorr::System
{
    void TransformSystem::Tick(float deltaTime)
    {
        ZoneScoped;
        
        auto view = m_pRegistry->view<Component::Transform>();

        for (auto entity : view)
        {
            auto &transform = view.get<Component::Transform>(entity);

            transform.Matrix = glm::mat4(1.0f);
            transform.Matrix = glm::translate(transform.Matrix, transform.Position);

            transform.Matrix = glm::translate(transform.Matrix, transform.RotationOrigin);
            transform.Matrix = glm::rotate(transform.Matrix, transform.Rotation, glm::vec3(0.0f, 0.0f, 1.f));
            transform.Matrix = glm::translate(transform.Matrix, -transform.RotationOrigin);
            
            transform.Matrix = glm::scale(transform.Matrix, transform.Size);
        }
    }

    void TransformSystem::Draw()
    {
    }

}  // namespace Lorr::System