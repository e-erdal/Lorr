//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

class Camera
{
public:
    Camera( const glm::vec3 &v3Pos, const glm::vec2 &v2Scale );

    void CalculateMetrices();

    void SetPosition( const glm::vec3 &v3Pos );
    void SetScale( const glm::vec2 &v2Scale );

public:
    const glm::mat4 GetTransform() const
    {
        return m_m4Projection * m_m4View;
    }

    const glm::vec3 &GetPosition() const
    {
        return m_v3Pos;
    }

    const glm::vec2 &GetScale() const
    {
        return m_v2Scale;
    }

private:
    glm::mat4 m_m4Projection{};
    glm::mat4 m_m4View{};

    glm::vec3 m_v3Pos{};
    glm::vec2 m_v2Scale{};

    float m_fRotation{};
    float m_fZoom{};
};