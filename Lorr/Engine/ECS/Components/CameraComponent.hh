//
// Created on Monday 6th December 2021 by e-erdal
//

#include "Engine/App/Engine.hh"
#include "Engine/ECS/Entity.hh"

namespace Lorr::Component
{
    struct Camera3DController
    {
        Camera3DController(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec3 &direction, const glm::vec3 &up, float fov, float zNear, float zFar)
        {
            m_Handle.Init(pos, size, direction, up, fov, zNear, zFar);
        }

        Camera3DController(const Camera3D &pCam) : m_Handle(pCam){};

        Camera3D m_Handle;
    };

    struct Camera2DController
    {
        Camera2DController(const glm::vec2 &pos, const glm::vec2 &size)
        {
            m_Handle.Init(pos, size);
        }

        Camera2DController(const Camera2D &pCam) : m_Handle(pCam){};

        Camera2D m_Handle;
    };

}  // namespace Lorr::Component