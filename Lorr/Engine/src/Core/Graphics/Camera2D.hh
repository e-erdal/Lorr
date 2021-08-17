//
// Created on July 1st 2021 by e-erdal.
//

#pragma once

#include <bgfx/bgfx.h>

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

    class Camera2D
    {
    public:
        void Init( const glm::vec2 &v2Pos, const glm::vec2 &v2Size );

        void CalculateView();
        void CalculateProjection();

        void SetPosition( const glm::vec2 &v2Pos );
        void SetScale( const glm::vec2 &v2Size );

        void SetUniformTransform( bgfx::ViewId view );

    public:
        const void *GetViewMatrixPtr() const
        {
            return &m_View[0][0];
        }

        const glm::mat4 GetProjection() const
        {
            return m_Projection;
        }

        const glm::mat4 GetView() const
        {
            return m_View;
        }

        const glm::vec2 &GetPosition() const
        {
            return m_Pos;
        }

    private:
        void OnResolutionChanged( uint32_t uWidth, uint32_t uHeight );

        glm::mat4 m_Projection{};
        glm::mat4 m_View{};

        glm::vec2 m_Pos{};
        glm::vec2 m_Scale{};

        float m_Rotation{};
        float m_Zoom{};
    };
}  // namespace Lorr