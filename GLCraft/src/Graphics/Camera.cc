#include "Camera.hh"

#include <glm/gtc/matrix_transform.hpp>

static glm::mat4 CalculateView( const glm::vec3 &v3Pos, float fRotation )
{
    return glm::inverse( glm::translate( glm::mat4( 1.f ), v3Pos ) * glm::rotate( glm::mat4( 1.f ), glm::radians( fRotation ), glm::vec3( .0f, .0f, 1.f ) ) );
}

static glm::mat4 CalculateProjection( const glm::vec2 &v2Scale )
{
    glm::mat4 proj = glm::ortho( 0.0f, glm::ceil( v2Scale.x ), glm::ceil( v2Scale.y ), .0f, 0.1f, 1000.f );
    return proj;
}

Camera::Camera( const glm::vec3 &v3Pos, const glm::vec2 &v2Scale ) : m_v3Pos( v3Pos ), m_v2Scale( v2Scale )
{
    CalculateMetrices();
}

void Camera::CalculateMetrices()
{
    m_m4Projection = CalculateProjection( m_v2Scale );
    m_m4View = CalculateView( m_v3Pos, m_fRotation );
}

void Camera::SetPosition( const glm::vec3 &v3Pos )
{
    m_v3Pos = v3Pos;
    m_m4View = CalculateView( m_v3Pos, m_fRotation );
}

void Camera::SetScale( const glm::vec2 &v2Scale )
{
    m_v2Scale = v2Scale;
    m_m4Projection = CalculateProjection( m_v2Scale );
}