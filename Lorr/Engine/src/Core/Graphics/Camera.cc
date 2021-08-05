#include "Camera.hh"

#include <DirectXMath.h>
#include <Engine.hh>

using namespace DirectX;

namespace Lorr
{
    void Camera::OnResolutionChanged( uint32_t uWidth, uint32_t uHeight )
    {
        SetScale( { uWidth, uHeight } );
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

        CalculateView();
        CalculateProjection();
    }

    void Camera::CalculateView()
    {
        ZoneScoped;

        m_m4View = glm::lookAtRH( m_v3Pos, m_v3Pos + m_v3Direction, m_v3Up );
    }

    void Camera::CalculateProjection()
    {
        ZoneScoped;

        m_m4Projection = glm::perspectiveFovRH_ZO( glm::radians( m_fFOV ), m_v2Size.x, m_v2Size.y, m_fZNear, m_fZFar );
    }

    void Camera::SetPosition( const glm::vec3 &v3Pos )
    {
        ZoneScoped;

        m_v3Pos = v3Pos;
        CalculateView();
    }

    void Camera::SetScale( const glm::vec2 &v2Size )
    {
        ZoneScoped;

        m_v2Size = v2Size;
        m_fAspect = m_v2Size.x / m_v2Size.y;

        CalculateProjection();
    }

    void Camera::StartMoving( Direction eState )
    {
        m_MovingDirection |= eState;
    }

    void Camera::StopMoving( Direction eState )
    {
        m_MovingDirection &= ~eState;
    }

    void Camera::SetDirection( float fOffsetX, float fOffsetY )
    {
        ZoneScoped;

        float offX = fOffsetX * 0.3f;
        float offY = fOffsetY * 0.3f;

        m_fAngleX += offX;
        m_fAngleY -= offY;

        // Block camera direction so it wont fuck up
        if ( m_fAngleY > 89.f ) m_fAngleY = 89.f;
        if ( m_fAngleY < -89.f ) m_fAngleY = -89.f;

        glm::vec3 front;
        front.x = cos( glm::radians( m_fAngleX ) ) * cos( glm::radians( m_fAngleY ) );
        front.y = sin( glm::radians( m_fAngleY ) );
        front.z = sin( glm::radians( m_fAngleX ) ) * cos( glm::radians( m_fAngleY ) );
        m_v3Direction = glm::normalize( front );

        m_v3Right = glm::normalize( glm::cross( m_v3Direction, { 0, 1, 0 } ) );
        m_v3Up = glm::normalize( glm::cross( m_v3Right, m_v3Direction ) );

        CalculateView();
    }

    void Camera::Update( float fDelta )
    {
        ZoneScoped;

        auto lastPos = m_v3Pos;

        if ( m_MovingDirection & Direction::FORWARD ) m_v3Pos += m_v3Direction * m_fMovingSpeed * fDelta;
        if ( m_MovingDirection & Direction::BACKWARD ) m_v3Pos -= m_v3Direction * m_fMovingSpeed * fDelta;
        if ( m_MovingDirection & Direction::LEFT ) m_v3Pos -= m_v3Right * m_fMovingSpeed * fDelta;
        if ( m_MovingDirection & Direction::RIGHT ) m_v3Pos += m_v3Right * m_fMovingSpeed * fDelta;
        if ( m_MovingDirection & Direction::UP ) m_v3Pos.y += m_fMovingSpeed * fDelta;
        if ( m_MovingDirection & Direction::DOWN ) m_v3Pos.y -= m_fMovingSpeed * fDelta;

        if ( m_v3Pos != lastPos ) CalculateView();
    }
}  // namespace Lorr