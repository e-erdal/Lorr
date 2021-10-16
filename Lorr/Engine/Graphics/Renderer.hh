//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Engine/Core/Window/PlatformWindow.hh"

#include "Engine/Graphics/Common/ITexture.hh"
#include "Engine/Graphics/Common/IRenderer.hh"

namespace Lorr
{
    class Renderer
    {
    public:
        ~Renderer();

        void Init(PlatformWindow *pWindow);

        void BeginFrame();
        void EndFrame();
        void Draw(uint32_t indexCount);

        void SetViewTransform(const glm::mat4 &proj, const glm::mat4 &view);
        void SetVSyncState(bool vSync);

        void Submit(const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color);

        void Reset();
        void SetView(uint32_t width, uint32_t height);

    public:
        const auto &GetPlaceholder() const
        {
            return m_PlaceholderTexture;
        }

    private:
        IRenderer *m_pHandle = 0;

        TextureHandle m_PlaceholderTexture = 0;

        uint32_t m_ResetFlags = 0;
        uint32_t m_ResWidth = 0;
        uint32_t m_ResHeight = 0;

        uintptr_t m_State = 0;
    };

}  // namespace Lorr