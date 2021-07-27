#include "Camera.hh"

#include <glm/gtc/matrix_transform.hpp>

#include <Engine.hh>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Lorr
{
    void Camera::OnResolutionChanged( uint32_t uWidth, uint32_t uHeight )
    {
        m_v2Size = { uWidth, uHeight };
        m_fAspect = m_v2Size.x / m_v2Size.y;

        CalculateMetrices();
    }

    void Camera::Init( const glm::vec3 &v3Pos, const glm::vec3 &v3Direction, const glm::vec3 &v3Up, const glm::vec2 &v2Size, float fFOV, float fZNear, float fZFar )
    {
        ZoneScoped;

        GetEngine()->GetWindow()->OnResolutionChanged.connect<&Camera::OnResolutionChanged>( this );

        m_v3Pos = v3Pos;
        m_v3Direction = v3Direction;
        m_v3Up = v3Up;
        m_v2Size = v2Size;
        m_fFOV = fFOV;
        m_fZNear = fZNear;
        m_fZFar = fZFar;
        m_fAspect = v2Size.x / v2Size.y;

        CalculateMetrices();
    }

    void Camera::CalculateMetrices()
    {
        ZoneScoped;

        m_m4View = glm::lookAtRH( m_v3Pos, m_v3Pos + m_v3Direction, { 0, 1, 0 } );
        m_m4Projection = glm::perspectiveFovRH_ZO( glm::radians( m_fFOV ), m_v2Size.x, m_v2Size.y, m_fZNear, m_fZFar );
    }

    void Camera::SetPosition( const glm::vec3 &v3Pos )
    {
        ZoneScoped;

        m_v3Pos = v3Pos;
        CalculateMetrices();
    }

    void Camera::SetScale( const glm::vec2 &v2Size )
    {
        ZoneScoped;

        m_v2Size = v2Size;
        CalculateMetrices();
    }
}  // namespace Lorr