//
// Created on Sunday 3rd October 2021 by e-erdal
//

#pragma once

#include "Engine/ECS/Entity.hh"

namespace Lorr::Component
{
    struct Transform
    {
        glm::vec3 Position;
        glm::vec3 Size;
        float Rotation = 0.f;

        glm::mat4 Matrix;
    };

}  // namespace Lorr::Component