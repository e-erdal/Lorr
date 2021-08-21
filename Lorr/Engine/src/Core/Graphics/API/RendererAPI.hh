//
// Created on August 20th 2021 by e-erdal.
//

#pragma once

#include "Core/Window/PlatformWindow.hh"

namespace Lorr
{
    enum class APIType
    {
        None,
        D3D11,
    };

    class Engine;

    class RendererAPI
    {
    public:
        RendererAPI() = default;

        virtual bool Init(PlatformWindow *pWindow, int width, int height) = 0;

        virtual void ChangeResolution(uint32_t width, uint32_t height) = 0;
        virtual void SetViewport(int width, int height, float farZ, float nearZ) = 0;
        virtual void SetClearColor(const glm::vec4 &color) = 0;
        virtual void Frame(uint32_t interval) = 0;
    public:
        static APIType CurrentAPI()
        {
            return m_CurrentAPI;
        }

    protected:
        static APIType m_CurrentAPI;
    };

}  // namespace Lorr