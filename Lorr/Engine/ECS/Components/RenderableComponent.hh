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
        TextureHandle texture = 0;
        glm::vec4 TextureCoords = { 1, 1, 1, 1 };
        
        glm::ivec4 Color = { 255, 255, 255, 255 };
    };

}  // namespace Lorr::Component