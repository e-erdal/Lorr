#include "Camera.hh"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( const glm::vec3 &v3Pos, const glm::vec3 &v3Up, const glm::vec2 &v2Size, float fFOV, float fZNear, float fZFar ) :
    m_v3Pos( v3Pos ), m_v3WorldUp( v3Up ), m_v2Size( v2Size ), m_fFOV( fFOV ), m_fZNear( fZNear ), m_fZFar( fZFar ), m_v3Angle( { 0, -90, 0 } ),
    m_fAspect( v2Size.x / v2Size.y )
{
    CalculateMetrices();
}

void Camera::CalculateMetrices()
{
    float pitch = glm::radians( m_v3Angle.x );
    float yaw = glm::radians( m_v3Angle.y );
    auto direction = glm::vec3{
        cos( pitch ) * cos( yaw ),
        sin( pitch ),
        cos( pitch ) * sin( yaw ),
    };

    m_v3Front = glm::normalize( direction );
    m_v3Right = glm::normalize( glm::cross( m_v3Front, m_v3WorldUp ) );
    m_v3Up = glm::normalize( glm::cross( m_v3Right, m_v3Front ) );

    m_m4Projection = glm::perspective( glm::radians( m_fFOV ), m_fAspect, m_fZNear, m_fZFar );
    m_m4View = glm::lookAt( m_v3Pos, m_v3Pos + m_v3Front, m_v3Up );
}

void Camera::SetPosition( const glm::vec3 &v3Pos )
{
    m_v3Pos = v3Pos;
    CalculateMetrices();
}

void Camera::SetScale( const glm::vec2 &v2Size )
{
    m_v2Size = v2Size;
    CalculateMetrices();
}