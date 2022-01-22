//
// Created on Monday 4th October 2021 by e-erdal
//

#pragma once

#include "Engine/ECS/ISystem.hh"

namespace lr::System
{
    class Renderer2DSystem : public ISystem
    {
    public:
        Renderer2DSystem(EntityRegistry *pRegistry) : ISystem(pRegistry){};
        void Init() override;
        void Tick(float deltaTime) override;
        void Draw() override;
    };

}  // namespace lr::System