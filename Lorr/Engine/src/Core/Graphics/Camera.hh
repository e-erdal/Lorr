//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

namespace Lorr
{
    class Camera
    {
    public:
        void Init( const glm::vec3 &v3Pos, const glm::vec3 &v3Up, const glm::vec2 &v2Size, float fFOV, float fZNear, float fZFar );

        void CalculateMetrices();

        void SetPosition( const glm::vec3 &v3Pos );
        void SetScale( const glm::vec2 &v2Size );

    public:
        const glm::mat4 GetTransform() const
        {
            return m_m4Projection * m_m4View;
        }

        const glm::vec3 &GetPosition() const
        {
            return m_v3Pos;
        }

    private:
        glm::mat4 m_m4Projection{};
        glm::mat4 m_m4View{};

        glm::vec3 m_v3Pos{};
        glm::vec3 m_v3Angle{};
        glm::vec3 m_v3WorldUp{};
        glm::vec3 m_v3Front{};
        glm::vec3 m_v3Right{};
        glm::vec3 m_v3Up{};

        glm::vec2 m_v2Size{};

        float m_fFOV{};
        float m_fAspect{};
        float m_fZNear{};
        float m_fZFar{};
    };
}  // namespace Lorr