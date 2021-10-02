//
// Created on August 8th 2021 by e-erdal.
//

#pragma once

#include "Engine/Core/Window/PlatformWindow.hh"

#include "Engine/Graphics/Texture2D.hh"

namespace Lorr
{
    class Renderer2D
    {
    public:
        ~Renderer2D();

        void Init(PlatformWindow *pWindow);

        void BeginFrame();
        void EndFrame();

        void SetViewTransform(uint32_t viewID, const glm::mat4 &proj, const glm::mat4 &view);
        void SetTexture(Texture2D *pTexture, const bgfx::UniformHandle &uniform);
        void SetVSyncState(bool VSync);

        void Submit(const glm::mat4 &transform, const glm::vec4 &uv, const glm::ivec4 &color);

        void Reset();
    public:
        const auto &GetPlaceholder() const
        {
            return m_pPlaceholderTexture;
        }

    private:
        Texture2D *m_pPlaceholderTexture = 0;

        uint32_t m_ResetFlags = 0;
        uint32_t m_ResWidth = 0;
        uint32_t m_ResHeight = 0;

        uintptr_t m_State = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA;
    };

}  // namespace Lorr