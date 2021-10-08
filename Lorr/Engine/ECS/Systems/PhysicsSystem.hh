//
// Created on Thursday 7th October 2021 by e-erdal
//

#pragma once

#include "Engine/ECS/ISystem.hh"

namespace Lorr::System
{
    class PhysicsSystem : public ISystem
    {
    public:
        PhysicsSystem(EntityRegistry *pRegistry) : ISystem(pRegistry){};
        void Tick(float deltaTime) override;
        void Draw() override;
    };

}  // namespace Lorr::System