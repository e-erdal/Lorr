//
// Created on August 20th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/D3D/D3DType.hh"
#include "Engine/Core/Window/PlatformWindow.hh"
#include "Engine/Graphics/Common/ITexture.hh"

namespace Lorr
{
    enum class APIType : uint8_t
    {
        None,
        D3D11,
        D3D12,
    };

    enum RendererReset : uint32_t
    {
        RESET_DEPTH_TESTING = 1 << 0,
        RESET_RASTER_CULL = 1 << 1,
        RESET_ALPHA_BLENDING = 1 << 3,
    };
    BitFlags(RendererReset);

    class Engine;
    class IRenderer
    {
    public:
        IRenderer() = default;

        virtual bool Init(PlatformWindow *pWindow, uint32_t width, uint32_t height) = 0;

        virtual void ChangeResolution(uint32_t width, uint32_t height) = 0;

        virtual void SetViewport(uint32_t width, uint32_t height, float farZ, float nearZ) = 0;
        virtual void SetClearColor(const glm::vec4 &color) = 0;
        virtual void SetScissor(const glm::vec4 &lrtb) = 0;
        virtual void SetDepthFunc(D3D::DepthFunc func, bool depthEnabled) = 0;
        virtual void SetCulling(D3D::Cull cull, bool counterClockwise) = 0;
        virtual void SetBlend(bool enableBlending, bool alphaCoverage) = 0;
        virtual void CreateTarget(const Identifier &ident, uint32_t width, uint32_t height, TextureHandle texture = 0) = 0;
        virtual void SetCurrentTarget(const Identifier &ident) = 0;
        virtual TextureHandle GetTargetTexture(const Identifier &ident) = 0;

        virtual void Frame(uint32_t interval) = 0;
        virtual void HandlePreFrame() = 0;

        virtual void DrawIndexed(uint32_t indexCount, uint32_t startIndex = 0, uint32_t baseVertex = 0) = 0;

    public:
        static APIType CurrentAPI()
        {
            return m_CurrentAPI;
        }

        const TextureHandle GetPlaceholder() const
        {
            return m_PlaceholderTexture;
        }

    protected:
        static APIType m_CurrentAPI;
        TextureHandle m_PlaceholderTexture = 0;

        RendererReset m_ResetFlags;
    };

}  // namespace Lorr