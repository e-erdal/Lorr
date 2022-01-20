//
// Created on August 20th 2021 by e-erdal.
/// Renderer will be refactored after D3D12.
//

#pragma once

#include "Engine/Core/Window/PlatformWindow.hh"
#include "Engine/Graphics/Types.hh"
#include "Engine/Graphics/D3D/D3DType.hh"

#include "BaseRenderBuffer.hh"
#include "BaseShader.hh"
#include "BaseTexture.hh"

#include "Engine/Graphics/PostProcess/IPostProcess.hh"

namespace lr
{
    /// Renderer configurations
    namespace Renderer::Config
    {
        constexpr u32 kMaxFrameBuffers = 3;
    }

    class Engine;
    class BaseRenderer
    {
    public:
        BaseRenderer() = default;

        virtual bool Init(PlatformWindow *pWindow, u32 width, u32 height) = 0;

        /// SWAPCHAIN ///
        virtual void Resize(u32 width, u32 height) = 0;

        /// RS STAGE ///
        virtual void SetViewport(u32 width, u32 height, float farZ, float nearZ) = 0;
        virtual void SetScissor(const glm::vec4 &lrtb) = 0;
        virtual void SetDepthFunc(DepthFunc func, bool depthEnabled) = 0;
        virtual void SetCulling(Cull cull, bool counterClockwise) = 0;
        virtual void SetWireframeState(bool enabled) = 0;
        virtual void SetBlend(bool enableBlending, bool alphaCoverage) = 0;
        virtual void SetPrimitiveType(PrimitiveType type) = 0;

        /// RENDER TARGETS ///
        virtual void CreateRenderTarget(const Identifier &ident, u32 width, u32 height, TextureHandle texture = 0, u32 mipLevels = 1) = 0;
        virtual void SetRenderTarget(const Identifier &ident) = 0;
        virtual void SetRenderTargetClearColor(const Identifier &ident, const glm::vec4 &color) = 0;
        virtual TextureHandle GetRenderTargetTexture(const Identifier &ident) = 0;

        /// IA STAGE ///
        virtual void SetVertexBuffer(RenderBufferHandle buffer, InputLayout *pLayout, u32 offset = 0) = 0;
        virtual void SetIndexBuffer(RenderBufferHandle buffer, bool index32 = true, u32 offset = 0) = 0;

        /// RENDER BUFFERS ///
        virtual void SetConstantBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) = 0;

        virtual void SetShaderResource(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) = 0;
        virtual void SetUAVResource(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) = 0;
        
        virtual void SetShaderResource(TextureHandle texture, RenderBufferTarget target, u32 slot) = 0;
        virtual void SetUAVResource(TextureHandle texture, RenderBufferTarget target, u32 slot) = 0;
        virtual void SetSamplerState(TextureHandle texture, RenderBufferTarget target, u32 slot) = 0;

        virtual void MapBuffer(RenderBufferHandle buffer, void *pData, u32 dataSize) = 0;
        virtual void TransferResourceData(RenderBufferHandle inputBuffer, RenderBufferHandle outputBuffer) = 0;
        virtual void TransferResourceData(TextureHandle inputTexture, TextureHandle outputTexture) = 0;

        /// SHADERS  ///
        virtual void SetShader(ShaderHandle shader) = 0;

        /// DRAWING STAGE ///
        virtual void BeginFrame() = 0;
        virtual void Frame() = 0;
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
        PostProcessManager m_PostProcessMan;
    };

}  // namespace lr