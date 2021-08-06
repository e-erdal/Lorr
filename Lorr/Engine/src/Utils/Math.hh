//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace Lorr::Math
{
    constexpr float PI = 3.14159265358979323f;
    constexpr float PI2 = 6.283185307f;
    constexpr float PIDIV2 = 1.570796327f;
    constexpr float E = 2.71828182846f;

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

    // DXDMATH function
    inline void ScalarSinCos( float *pSin, float *pCos, float fVal )
    {
        float quotient = 0.15915494309189533577f * fVal;
        if ( fVal >= 0.0f )
        {
            quotient = (float) ( (int) ( quotient + 0.5f ) );
        }
        else
        {
            quotient = (float) ( (int) ( quotient - 0.5f ) );
        }

        float y = fVal - PI2 * quotient;
        float sign;

        if ( y > PIDIV2 )
        {
            y = PI - y;
            sign = -1.0f;
        }
        else if ( y < -PIDIV2 )
        {
            y = -PI - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        float y2 = y * y;

        *pSin = ( ( ( ( ( -2.3889859e-08f * y2 + 2.7525562e-06f ) * y2 - 0.00019840874f ) * y2 + 0.0083333310f ) * y2 - 0.16666667f ) * y2 + 1.0f ) * y;
        float p = ( ( ( ( -2.6051615e-07f * y2 + 2.4760495e-05f ) * y2 - 0.0013888378f ) * y2 + 0.041666638f ) * y2 - 0.5f ) * y2 + 1.0f;
        *pCos = sign * p;
    }
}  // namespace Lorr::Math