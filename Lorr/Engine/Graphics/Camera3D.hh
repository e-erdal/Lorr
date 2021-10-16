//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

namespace Lorr
{
    class Camera3D
    {
    public:
        void Init(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec3 &direction, const glm::vec3 &up, float fov, float zNear,
                  float zFar);

        void CalculateView();
        void CalculateProjection();

        void SetPosition(const glm::vec3 &pos);
        void SetSize(const glm::vec2 &size);

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

    private:
        void OnResolutionChanged(uint32_t width, uint32_t height);

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
        float m_Rotation{};
        float m_Zoom{};
    };
}  // namespace Lorr