
//
// Created on July 6th 2021 by e-erdal.
//

#pragma once

#if LR_BACKEND_D3D11

#include <TracyD3D11.hpp>

#include "Engine/Graphics/Common/BaseRenderer.hh"
#include "Engine/Graphics/Common/BaseShader.hh"
#include "Engine/Graphics/Common/BaseTexture.hh"

#include "D3D11SwapChain.hh"
#include "D3D11RendererStateManager.hh"
#include "D3D11RenderTargetManager.hh"

namespace lr
{
    class D3D11Texture;
    class D3D11Renderer : public BaseRenderer
    {
    public:
        bool Init(PlatformWindow *pWindow, u32 width, u32 height) override;

        /// SWAPCHAIN ///
        void Resize(u32 width, u32 height) override;

        /// RS STAGE ///
        void SetViewport(u32 width, u32 height, float farZ, float nearZ) override;
        void SetScissor(const glm::vec4 &lrtb) override;
        void SetDepthFunc(DepthFunc func, bool depthEnabled) override;
        void SetCulling(Cull cull, bool counterClockwise) override;
        void SetWireframeState(bool enabled) override;
        void SetBlend(bool enableBlending, bool alphaCoverage) override;
        void SetPrimitiveType(PrimitiveType type) override;

        /// RENDER TARGETS ///
        void CreateRenderTarget(const Identifier &ident, u32 width, u32 height, TextureHandle texture = 0, u32 mipLevels = 1) override;
        void SetRenderTarget(const Identifier &ident) override;
        void SetRenderTargetClearColor(const Identifier &ident, const glm::vec4 &color) override;
        TextureHandle GetRenderTargetTexture(const Identifier &ident) override;

        /// IA STAGE ///
        void SetVertexBuffer(RenderBufferHandle buffer, InputLayout *pLayout, u32 offset = 0) override;
        void SetIndexBuffer(RenderBufferHandle buffer, bool index32 = true, u32 offset = 0) override;

        /// RENDER BUFFERS ///
        void SetConstantBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) override;

        void SetShaderResource(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) override;
        void SetUAVResource(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) override;

        void SetShaderResource(TextureHandle texture, RenderBufferTarget target, u32 slot) override;
        void SetUAVResource(TextureHandle texture, RenderBufferTarget target, u32 slot) override;
        void SetSamplerState(TextureHandle texture, RenderBufferTarget target, u32 slot) override;

        void MapBuffer(RenderBufferHandle buffer, void *pData, u32 dataSize) override;
        void TransferResourceData(RenderBufferHandle inputBuffer, RenderBufferHandle outputBuffer) override;
        void TransferResourceData(TextureHandle inputTexture, TextureHandle outputTexture) override;

        /// SHADERS  ///
        void SetShader(ShaderHandle shader) override;

        /// DRAWING STAGE ///
        void BeginFrame() override;
        void Frame() override;
        void DrawIndexed(u32 indexCount, u32 startIndex, u32 baseVertex) override;
        void Dispatch(u32 thrX, u32 thrY, u32 thrZ) override;

        static D3D11Renderer *&Get();

    private:
        bool CreateDevice();
        bool CreateSwapChain(PlatformWindow *pWindow, u32 width, u32 height);
        bool CreateBackBuffer();
        bool CreateDepthTexture(u32 width, u32 height);
        bool CreateDepthStencil();
        bool CreateBlendState();
        bool CreateRasterizer();

    public:
        ID3D11Device *GetDevice() const
        {
            return m_pDevice;
        }

        ID3D11DeviceContext *GetDeviceContext() const
        {
            return m_pContext;
        }

    private:
        //* Main API *//
        ID3D11Device *m_pDevice = nullptr;
        ID3D11DeviceContext *m_pContext = nullptr;
        D3D11SwapChain *m_pSwapChain = nullptr;
        TracyD3D11Ctx m_TracyContext = nullptr;

        //* API Managers *//
        D3D11RendererStateManager m_StateManager;
        D3D11RenderTargetManager m_TargetManager;

        //* View handles *//
        D3D11Texture *m_pDepthTexture = nullptr;

        //* Current API states *//
        D3D11_BLEND_DESC m_BlendDesc = {};
        D3D11_RASTERIZER_DESC m_RasterizerDesc = {};
        D3D11_DEPTH_STENCIL_DESC m_DepthStencilDesc = {};

        ID3D11BlendState *m_pBlendState = nullptr;
        ID3D11RasterizerState *m_pRasterizerState = nullptr;
        ID3D11DepthStencilState *m_pDepthStencilState = nullptr;

        bool m_NeedToPresent = true;
        bool m_IsContextReady = false;
    };

}  // namespace lr

#define DX11Renderer D3D11Renderer::Get()

#endif