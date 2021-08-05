//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

namespace Lorr
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
    BitFlags( Direction );

    class Camera
    {
    public:
        void Init( const glm::vec3 &v3Pos, const glm::vec3 &v3Direction, const glm::vec3 &v3Up, const glm::vec2 &v2Size, float fFOV, float fZNear, float fZFar );

        void CalculateView();
        void CalculateProjection();

        void SetPosition( const glm::vec3 &v3Pos );
        void SetScale( const glm::vec2 &v2Size );

        // Tell camera to move to the direction we want (supports bitwise operators)
        void StartMoving( Direction eState );
        // Tell camera to stop moving to the direction we want (supports bitwise operators)
        void StopMoving( Direction eState );

        // Tell camera to look at where we want (offsets are not degrees)
        void SetDirection( float fOffsetX, float fOffsetY );

        // This function handles every camera stuff including movement
        void Update( float fDelta );

    public:
        const glm::mat4 GetProjection() const
        {
            return m_m4Projection;
        }

        const glm::mat4 GetView() const
        {
            return m_m4View;
        }

        const glm::vec3 &GetPosition() const
        {
            return m_v3Pos;
        }

        const float &GetFOV() const
        {
            return m_fFOV;
        }

        const float &GetAspectRatio() const
        {
            return m_fAspect;
        }

    private:
        void OnResolutionChanged( uint32_t uWidth, uint32_t uHeight );

        glm::mat4 m_m4Projection{};
        glm::mat4 m_m4View{};

        glm::vec3 m_v3Pos{};
        glm::vec3 m_v3Direction{};
        glm::vec3 m_v3Up{};
        glm::vec3 m_v3Right{};

        glm::vec2 m_v2Size{};

        float m_fFOV{};
        float m_fAspect{};
        float m_fZNear{};
        float m_fZFar{};
        float m_fMovingSpeed = 200.f;
        float m_fAngleX = 0, m_fAngleY = 0;

        Direction m_MovingDirection = Direction::NONE;
    };
}  // namespace Lorr