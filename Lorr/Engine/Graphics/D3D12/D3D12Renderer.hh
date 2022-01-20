//
// Created on Friday 22nd October 2021 by e-erdal
//! EXPERIMENTAL DIRECTX 12 RENDERER
//

#pragma once

#if LR_BACKEND_D3D12

#include "Engine/Graphics/Common/BaseRenderer.hh"
#include "Engine/Graphics/Common/BaseShader.hh"
#include "Engine/Graphics/Common/BaseTexture.hh"

#include "D3D12SwapChain.hh"
#include "D3D12RenderTargetManager.hh"

namespace lr
{
    class D3D12Renderer : public BaseRenderer
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

        static D3D12Renderer *&Get();

    private:
        /// Device creation section ///
        bool SelectAdapter(bool highPerformance);
        D3D_FEATURE_LEVEL SelectMaxFeatureLevel();
        bool CreateDevice();

    public:
        ID3D12Device *GetDevice() const
        {
            return m_pDevice;
        }

        D3D12SwapChain *GetSwapChain() const
        {
            return m_pSwapChain;
        }

    private:
        /// D3D12 DEVICE ///
        ID3D12Device *m_pDevice = nullptr;
        D3D_FEATURE_LEVEL m_MaxFeatureLevel;
        IDXGIFactory4 *m_pFactory = nullptr;
        IDXGIAdapter1 *m_pAdapter = nullptr;

        /// D3D12 SWAP CHAIN ///
        D3D12SwapChain *m_pSwapChain = nullptr;

        //* Current API states *//
        D3D12_BLEND_DESC m_BlendDesc = {};
        D3D12_RASTERIZER_DESC m_RasterizerDesc = {};
        D3D12_DEPTH_STENCIL_VIEW_DESC m_DepthStencilViewDesc = {};
        D3D12_DEPTH_STENCIL_DESC m_DepthStencilDesc = {};

        D3D12RenderTargetManager m_TargetManager;

        bool m_IsContextReady = false;
    };

#define DX12Renderer D3D12Renderer::Get()

}  // namespace lr

#endif