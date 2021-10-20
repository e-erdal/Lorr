#include "Camera2D.hh"

#include "Engine/App/Engine.hh"

namespace Lorr
{
    void Camera2D::OnResolutionChanged(uint32_t width, uint32_t height)
    {
        SetSize({ width, height });
    }

    void Camera2D::Init(const glm::vec2 &pos, const glm::vec2 &size)
    {
        ZoneScoped;

        GetEngine()->GetWindow()->OnResolutionChanged.connect<&Camera2D::OnResolutionChanged>(this);

        m_Pos = pos;
        m_Size = size;

        CalculateMetrices();
    }

    void Camera2D::CalculateMetrices()
    {
        ZoneScoped;

        float L = m_Pos.x;
        float R = m_Pos.x + m_Size.x;
        float T = m_Pos.y;
        float B = m_Pos.y + m_Size.y;

        m_Matrix = {
            { 2.0f / (R - L), 0.0f, 0.0f, 0.0f },
            { 0.0f, 2.0f / (T - B), 0.0f, 0.0f },
            { 0.0f, 0.0f, 0.5f, 0.0f },
            { (R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f },
        };
    }

    void Camera2D::SetPosition(const glm::vec2 &pos)
    {
        ZoneScoped;

        m_Pos = pos;

        CalculateMetrices();
    }

    void Camera2D::SetSize(const glm::vec2 &size)
    {
        ZoneScoped;

        m_Size = size;

        CalculateMetrices();
    }

}  // namespace Lorr