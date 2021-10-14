#include "Camera2D.hh"

#include "Engine/App/Engine.hh"

namespace Lorr
{
    void Camera2D::OnResolutionChanged(uint32_t width, uint32_t height)
    {
        SetScale({ width, height });
    }

    void Camera2D::Init(const glm::vec2 &pos, const glm::vec2 &size)
    {
        ZoneScoped;

        GetEngine()->GetWindow()->OnResolutionChanged.connect<&Camera2D::OnResolutionChanged>(this);

        m_Pos = pos;
        m_Scale = size;

        CalculateView();
        CalculateProjection();
    }

    void Camera2D::CalculateView()
    {
        ZoneScoped;

        m_View =
            glm::inverse(glm::translate(glm::mat4(1.f), glm::vec3(m_Pos.x, m_Pos.y, .0f)) * glm::rotate(glm::mat4(1.f), glm::radians(0.f), glm::vec3(.0f, .0f, 1.f)));
    }

    void Camera2D::CalculateProjection()
    {
        ZoneScoped;

        m_Projection = glm::orthoLH_ZO(0.0f, m_Scale.x, m_Scale.y, .0f, 0.1f, 10000.f);
        m_Projection[3].z = 1.f;

        GetEngine()->GetRenderer()->SetView(m_Scale.x, m_Scale.y);
    }

    void Camera2D::SetPosition(const glm::vec2 &pos)
    {
        ZoneScoped;

        m_Pos = pos;

        CalculateView();
    }

    void Camera2D::SetScale(const glm::vec2 &size)
    {
        ZoneScoped;

        m_Scale = size;

        CalculateProjection();
    }

}  // namespace Lorr