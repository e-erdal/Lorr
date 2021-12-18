//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

namespace lr
{
    class Camera2D
    {
    public:
        void Init(const glm::vec2 &pos, const glm::vec2 &size);

        void CalculateMetrices();

        void SetPosition(const glm::vec2 &pos);
        void SetSize(const glm::vec2 &size);

    public:
        const glm::mat4 &GetMatrix() const
        {
            return m_Matrix;
        }

    private:
        glm::mat4 m_Matrix{};

        glm::vec2 m_Pos{};
        glm::vec2 m_Size{};

        float m_Rotation{};
        float m_Zoom{};
    };
}  // namespace lr