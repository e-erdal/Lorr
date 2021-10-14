//
// Created on Sunday 3rd October 2021 by e-erdal
//

#pragma once

#include "Engine/ECS/Entity.hh"

#include "Engine/Graphics/Common/ITexture.hh"

namespace Lorr::Component
{
    struct Renderable
    {
        bool IsBatch = false;
        glm::ivec4 Color = { 255, 255, 255, 255 };

        TextureHandle texture = 0;
        glm::vec4 TextureCoords = { 1, 1, 1, 1 };
    };

}  // namespace Lorr::Component