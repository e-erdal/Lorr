#include "Camera2D.hh"

#include "Engine.hh"

namespace Lorr
{
    void Camera2D::OnResolutionChanged( uint32_t uWidth, uint32_t uHeight )
    {
        SetScale( { uWidth, uHeight } );
    }

    void Camera2D::Init( const glm::vec2 &v2Pos, const glm::vec2 &v2Size )
    {
        ZoneScoped;

        GetEngine()->GetWindow()->OnResolutionChanged.connect<&Camera2D::OnResolutionChanged>( this );

        m_Pos = v2Pos;
        m_Scale = v2Size;

        CalculateView();
        CalculateProjection();
    }

    void Camera2D::CalculateView()
    {
        ZoneScoped;

        m_View = glm::inverse( glm::translate( glm::mat4( 1.f ), glm::ceil( glm::vec3( m_Pos.x, m_Pos.y, .0f ) ) )
                               * glm::rotate( glm::mat4( 1.f ), glm::radians( 0.f ), glm::vec3( .0f, .0f, 1.f ) ) );
    }

    void Camera2D::CalculateProjection()
    {
        ZoneScoped;

        m_Projection = glm::ortho( 0.0f, glm::ceil( m_Scale.x ), glm::ceil( m_Scale.y ), .0f, 0.1f, 1.f );
        m_Projection[3].z = 1.f;
    }

    void Camera2D::SetPosition( const glm::vec2 &v2Pos )
    {
        ZoneScoped;

        m_Pos = v2Pos;

        CalculateView();
    }

    void Camera2D::SetScale( const glm::vec2 &v2Size )
    {
        ZoneScoped;

        m_Scale = v2Size;

        CalculateProjection();
    }

    void Camera2D::SetUniformTransform( bgfx::ViewId view )
    {
        bgfx::setViewTransform( view, &m_View[0][0], &m_Projection[0][0] );
    }
}  // namespace Lorr