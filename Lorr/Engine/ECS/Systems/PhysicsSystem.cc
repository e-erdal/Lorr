#include "PhysicsSystem.hh"

#include "Engine/ECS/Components/TransformComponent.hh"
#include "Engine/ECS/Components/PhysicsComponent.hh"

namespace Lorr::System
{
    void PhysicsSystem::Tick(float deltaTime)
    {
        auto world = GetEngine()->GetPhyWorld();
        world->Step(deltaTime, 6, 2);

        m_pRegistry->view<Component::Transform>().each([&](auto entity, Component::Transform &transform) {
            if (m_pRegistry->has<Component::Physics>(entity))
            {
                auto &dyn = m_pRegistry->get<Component::Physics>(entity);
                transform.Position.x = glm::round(dyn.pBody->GetPosition().x * 32.f);
                transform.Position.y = glm::round(dyn.pBody->GetPosition().y * 32.f);
                transform.Rotation = dyn.pBody->GetAngle();
            }
        });
    }

    void PhysicsSystem::Draw()
    {
    }

}  // namespace Lorr::System