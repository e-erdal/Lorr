//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

namespace Lorr
{
    class Camera2D
    {
    public:
        void Init(const glm::vec2 &pos, const glm::vec2 &size);

        void CalculateView();
        void CalculateProjection();

        void SetPosition(const glm::vec2 &pos);
        void SetScale(const glm::vec2 &size);

    public:
        const glm::mat4 &GetProjection() const
        {
            return m_Projection;
        }

        const glm::mat4 &GetView() const
        {
            return m_View;
        }

        const glm::vec2 &GetPosition() const
        {
            return m_Pos;
        }

    private:
        void OnResolutionChanged(uint32_t width, uint32_t height);

        glm::mat4 m_Projection{};
        glm::mat4 m_View{};

        glm::vec2 m_Pos{};
        glm::vec2 m_Scale{};

        float m_Rotation{};
        float m_Zoom{};
    };
}  // namespace Lorr