//
// Created on Monday 4th October 2021 by e-erdal
//

#pragma once

#include "Engine/ECS/ISystem.hh"

namespace Lorr::System
{
    class RendererSystem : public ISystem
    {
    public:
        RendererSystem(EntityRegistry *pRegistry) : ISystem(pRegistry){};
        void Tick(float deltaTime) override;
        void Draw() override;
    };

}  // namespace Lorr::System