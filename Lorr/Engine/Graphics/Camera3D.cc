#include "Camera3D.hh"

#include "Engine/App/Engine.hh"

namespace Lorr
{
    void Camera3D::OnResolutionChanged(uint32_t width, uint32_t height)
    {
        SetSize({ width, height });
    }

    void Camera3D::Init(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec3 &direction, const glm::vec3 &up, float fov, float zNear, float zFar)
    {
        ZoneScoped;

        GetEngine()->GetWindow()->OnResolutionChanged.connect<&Camera3D::OnResolutionChanged>(this);

        m_Pos = pos;
        m_Size = size;
        m_Direction = direction;
        m_Up = up;
        m_Fov = fov;
        m_zNear = zNear;
        m_zFar = zFar;
        m_Aspect = m_Size.x / m_Size.y;

        CalculateView();
        CalculateProjection();
    }

    void Camera3D::CalculateView()
    {
        ZoneScoped;

        m_View = glm::lookAtLH(m_Pos, m_Pos + m_Direction, m_Up);
    }

    void Camera3D::CalculateProjection()
    {
        ZoneScoped;

        m_Projection = glm::perspectiveFovLH_ZO(glm::radians(m_Fov), m_Size.x, m_Size.y, m_zNear, m_zFar);

        GetEngine()->GetRenderer()->SetView(m_Size.x, m_Size.y);
    }

    void Camera3D::SetPosition(const glm::vec3 &pos)
    {
        ZoneScoped;

        m_Pos = pos;

        CalculateView();
    }

    void Camera3D::SetSize(const glm::vec2 &size)
    {
        ZoneScoped;

        m_Size = size;

        CalculateProjection();
    }

}  // namespace Lorr