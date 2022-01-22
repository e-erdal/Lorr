//
// Created on Monday 6th December 2021 by e-erdal
//

#pragma once

#include "Engine/ECS/ISystem.hh"

namespace lr::System
{
    class CameraControllerSystem : public ISystem
    {
    public:
        CameraControllerSystem(EntityRegistry *pRegistry) : ISystem(pRegistry){};
        void Init() override;
        void Tick(float deltaTime) override;
        void Draw() override;
    };

}  // namespace lr::System