//
// Created on Sunday 3rd October 2021 by e-erdal
//

#pragma once

#include "Engine/ECS/Entity.hh"

namespace lr::Component
{
    struct Transform
    {
        Transform(){};

        Transform(glm::vec3 pos, glm::vec3 size = {}, float rotation = 0.f) : Position(pos), Size(size)
        {
            Rotation = glm::radians(rotation);
            SetOriginTopLeft();
        }

        glm::vec3 Position;
        glm::vec3 Size;
        float Rotation = 0.f;
        glm::vec3 RotationOrigin;

        glm::mat4 Matrix;

        void SetOriginCenter()
        {
            RotationOrigin = Size / 2.f;
        }

        void SetOriginTopLeft()
        {
            RotationOrigin.x = 0;
            RotationOrigin.y = 0;
        }
    };

}  // namespace lr::Component