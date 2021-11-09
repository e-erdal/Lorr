//
// Created on Friday 29th October 2021 by e-erdal
//

#pragma once

#include "Engine/ECS/ISystem.hh"

namespace Lorr::System
{
    class ModelRendererSystem : public ISystem
    {
    public:
        ModelRendererSystem(EntityRegistry *pRegistry) : ISystem(pRegistry){};
        void Tick(float deltaTime) override;
        void Draw() override;
    };

}  // namespace Lorr::System