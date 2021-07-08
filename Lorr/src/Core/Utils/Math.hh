//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Math
{
    inline glm::mat4 CalcTransform( const glm::vec3 &v3Position, const glm::vec3 &v3Size, float fRotation = 0.f, const glm::vec3 &v3RotationAxis = { 0, 0, 1 } )
    {
        return glm::translate( glm::mat4( 1.f ), v3Position ) * glm::rotate( glm::mat4( 1.f ), fRotation, v3RotationAxis ) * glm::scale( glm::mat4( 1.f ), v3Size );
    }

    inline constexpr float Approach( float target, float current, float delta )
    {
        float deltaVel = target - current;
        if ( deltaVel > delta ) return current + delta;
        if ( deltaVel < -delta ) return current - delta;

        return target;
    }
}  // namespace Math