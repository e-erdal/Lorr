//
// Created on August 20th 2021 by e-erdal.
//

#pragma once

#include "Engine/Core/Window/PlatformWindow.hh"

namespace Lorr
{
    enum class APIType : uint8_t
    {
        None,
        D3D11,
        D3D12,
    };

    class Engine;
    class IRenderer
    {
    public:
        IRenderer() = default;

        virtual bool Init(PlatformWindow *pWindow, uint32_t width, uint32_t height) = 0;

        virtual void ChangeResolution(uint32_t width, uint32_t height) = 0;
        virtual void SetViewport(uint32_t width, uint32_t height, float farZ, float nearZ) = 0;
        virtual void SetClearColor(const glm::vec4 &color) = 0;
        virtual void Frame(uint32_t interval) = 0;
        virtual void HandlePreFrame() = 0;

        virtual void DrawIndexed(uint32_t indexCount) = 0;

    public:
        static APIType CurrentAPI()
        {
            return m_CurrentAPI;
        }

    protected:
        static APIType m_CurrentAPI;
    };

}  // namespace Lorr