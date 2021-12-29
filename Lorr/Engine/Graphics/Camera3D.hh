//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

namespace lr
{
    enum class Direction
    {
        NONE,
        FORWARD = 1 << 0,
        BACKWARD = 1 << 1,
        LEFT = 1 << 2,
        RIGHT = 1 << 3,
        UP = 1 << 4,
        DOWN = 1 << 5
    };
    BitFlags(Direction);

    class Camera3D
    {
    public:
        void Init(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec3 &direction, const glm::vec3 &up, float fov, float zNear, float zFar);

        void CalculateView();
        void CalculateProjection();

        void SetPosition(const glm::vec3 &pos);
        void SetSize(const glm::vec2 &size);

        void Move(Direction direction, bool stop);
        void SetDirection(float offX, float offY);

        void Update(float deltaTime);

    public:
        const glm::mat4 &GetProjection() const
        {
            return m_Projection;
        }

        const glm::mat4 &GetView() const
        {
            return m_View;
        }

        const glm::vec3 &GetPosition() const
        {
            return m_Pos;
        }

        float GetAspect()
        {
            return m_Aspect;
        }

    private:
        glm::mat4 m_Projection{};
        glm::mat4 m_View{};

        glm::vec3 m_Pos{};
        glm::vec3 m_Direction{};
        glm::vec3 m_Up{};
        glm::vec3 m_Right{};
        glm::vec2 m_Size{};

        float m_Fov = 0.f;
        float m_Aspect = 0.f;
        float m_zNear = 0.f;
        float m_zFar = 0.f;
        float m_MovingSpeed = 50.f;
        float m_AngleX = 0, m_AngleY = 0;

        Direction m_MovingDirection = Direction::NONE;
    };
}  // namespace lr