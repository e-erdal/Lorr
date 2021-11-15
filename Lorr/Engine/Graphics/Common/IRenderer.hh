//
// Created on August 20th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/Types.hh"
#include "Engine/Graphics/D3D/D3DType.hh"
#include "Engine/Core/Window/PlatformWindow.hh"

#include "IRenderBuffer.hh"
#include "IShader.hh"
#include "ITexture.hh"

namespace Lorr
{
    class Engine;
    class IRenderer
    {
    public:
        IRenderer() = default;

        virtual bool Init(PlatformWindow *pWindow, u32 width, u32 height) = 0;

        virtual void ChangeResolution(u32 width, u32 height) = 0;

        virtual void SetViewport(u32 width, u32 height, float farZ, float nearZ) = 0;
        virtual void SetClearColor() = 0;
        virtual void SetScissor(const glm::vec4 &lrtb) = 0;
        virtual void SetDepthFunc(D3D::DepthFunc func, bool depthEnabled) = 0;
        virtual void SetCulling(D3D::Cull cull, bool counterClockwise) = 0;
        virtual void SetBlend(bool enableBlending, bool alphaCoverage) = 0;
        virtual void CreateTarget(const Identifier &ident, u32 width, u32 height, TextureHandle texture = 0, u32 mipLevels = 1) = 0;
        virtual void SetCurrentTarget(const Identifier &ident) = 0;
        virtual TextureHandle GetTargetTexture(const Identifier &ident) = 0;

        virtual void UseVertexBuffer(RenderBufferHandle buffer, InputLayout *pLayout, u32 offset = 0) = 0;
        virtual void UseIndexBuffer(RenderBufferHandle buffer, bool index32 = true, u32 offset = 0) = 0;
        virtual void UseConstantBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) = 0;
        virtual void UseShaderBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) = 0;
        virtual void UseShaderBuffer(TextureHandle texture, RenderBufferTarget target, u32 slot) = 0;
        virtual void UseUAV(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) = 0;
        virtual void UseUAV(TextureHandle texture, RenderBufferTarget target, u32 slot) = 0;
        virtual void UseSampler(TextureHandle texture, RenderBufferTarget target, u32 slot) = 0;

        // virtual void MapBuffer(RenderBufferHandle buffer) = 0;
        // virtual void UnmapBuffer(RenderBufferHandle buffer) = 0;

        virtual void UseShader(ShaderHandle shader) = 0;

        virtual void TransferResourceData(RenderBufferHandle inputBuffer, RenderBufferHandle outputBuffer) = 0;
        virtual void TransferResourceData(TextureHandle inputTexture, TextureHandle outputTexture) = 0;

        virtual void Frame(u32 interval) = 0;
        virtual void HandlePreFrame() = 0;

        virtual void DrawIndexed(u32 indexCount, u32 startIndex = 0, u32 baseVertex = 0) = 0;
        virtual void Dispatch(u32 thrX, u32 thrY, u32 thrZ) = 0;

    public:
        static RendererType CurrentAPI()
        {
            return m_CurrentAPI;
        }

        const TextureHandle GetPlaceholder() const
        {
            return m_PlaceholderTexture;
        }

    protected:
        static RendererType m_CurrentAPI;
        TextureHandle m_PlaceholderTexture = 0;
    };

}  // namespace Lorr