#include "CameraControllerSystem.hh"

#include "Engine/ECS/Components/CameraComponent.hh"

namespace Lorr::System
{
    void CameraControllerSystem::Tick(float deltaTime)
    {
        ZoneScoped;
        
        auto view = m_pRegistry->view<Component::Camera3DController>();

        for (auto entity : view)
        {
            auto &handle = view.get<Component::Camera3DController>(entity);
            handle.m_Handle.Update(deltaTime);
        }
    }

    void CameraControllerSystem::Draw()
    {
    }

}  // namespace Lorr::System