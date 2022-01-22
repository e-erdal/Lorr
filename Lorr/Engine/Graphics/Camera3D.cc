#include "Camera3D.hh"

#include "Engine/App/Engine.hh"

namespace lr
{
    void Camera3D::Init(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec3 &direction, const glm::vec3 &up, float fov, float zNear,
                        float zFar)
    {
        ZoneScoped;

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

        m_View = glm::lookAtRH(m_Pos, m_Pos + m_Direction, m_Up);
    }

    void Camera3D::CalculateProjection()
    {
        ZoneScoped;

        m_Projection = glm::perspectiveFovRH_ZO(glm::radians(m_Fov), m_Size.x, m_Size.y, m_zNear, m_zFar);
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

    void Camera3D::Move(Direction direction, bool stop)
    {
        ZoneScoped;

        if (!stop)
            m_MovingDirection |= direction;
        else
            m_MovingDirection &= ~direction;
    }

    void Camera3D::SetDirection(float offX, float offY)
    {
        ZoneScoped;

        offX *= 0.3f;
        offY *= 0.3f;

        m_AngleX += offX;
        m_AngleY -= offY;

        // Block camera direction
        if (m_AngleY > 89.f) m_AngleY = 89.f;
        if (m_AngleY < -89.f) m_AngleY = -89.f;

        float radAnglX = glm::radians(m_AngleX);
        float radAnglY = glm::radians(m_AngleY);

        glm::vec3 front;
        front.x = cos(radAnglX) * cos(radAnglY);
        front.y = sin(radAnglY);
        front.z = sin(radAnglX) * cos(radAnglY);
        m_Direction = glm::normalize(front);

        m_Right = glm::normalize(glm::cross(m_Direction, { 0, 1, 0 }));
        m_Up = glm::normalize(glm::cross(m_Right, m_Direction));

        CalculateView();
    }

    void Camera3D::Update(float deltaTime)
    {
        ZoneScoped;

        auto lastPos = m_Pos;

        if (m_MovingDirection & Direction::FORWARD) m_Pos += m_Direction * m_MovingSpeed * deltaTime;
        if (m_MovingDirection & Direction::BACKWARD) m_Pos -= m_Direction * m_MovingSpeed * deltaTime;
        if (m_MovingDirection & Direction::LEFT) m_Pos -= m_Right * m_MovingSpeed * deltaTime;
        if (m_MovingDirection & Direction::RIGHT) m_Pos += m_Right * m_MovingSpeed * deltaTime;
        if (m_MovingDirection & Direction::UP) m_Pos.y += m_MovingSpeed * deltaTime;
        if (m_MovingDirection & Direction::DOWN) m_Pos.y -= m_MovingSpeed * deltaTime;

        if (m_Pos != lastPos) CalculateView();
    }

}  // namespace lr