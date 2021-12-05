//
// Created on Friday 22nd October 2021 by e-erdal
//! EXPERIMENTAL DIRECTX 12 RENDERER
//

#pragma once

#if LR_BACKEND_D3D12

#include "Engine/Graphics/Common/BaseRenderer.hh"
#include "Engine/Graphics/Common/BaseShader.hh"
#include "Engine/Graphics/Common/BaseTexture.hh"

#include "D3D12RenderTargetManager.hh"

namespace Lorr
{
    class D3D12Renderer : public BaseRenderer
    {
    public:
        bool Init(PlatformWindow *pWindow, u32 width, u32 height) override;

        void ChangeResolution(u32 width, u32 height) override;

        void SetViewport(u32 width, u32 height, float farZ, float nearZ) override;
        void SetClearColor() override;
        void SetScissor(const glm::vec4 &lrtb) override;
        void SetDepthFunc(D3D::DepthFunc func, bool depthEnabled) override;
        void SetCulling(D3D::Cull cull, bool counterClockwise) override;
        void SetBlend(bool enableBlending, bool alphaCoverage) override;

        void CreateTarget(const Identifier &ident, u32 width, u32 height, TextureHandle texture = 0, u32 mipLevels = 1) override;
        void SetCurrentTarget(const Identifier &ident) override;
        TextureHandle GetTargetTexture(const Identifier &ident) override;

        void UseVertexBuffer(RenderBufferHandle buffer, InputLayout *pLayout, u32 offset = 0) override;
        void UseIndexBuffer(RenderBufferHandle buffer, bool index32 = true, u32 offset = 0) override;
        void UseConstantBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) override;
        void UseShaderBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) override;
        void UseShaderBuffer(TextureHandle texture, RenderBufferTarget target, u32 slot) override;
        void UseUAV(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot) override;
        void UseUAV(TextureHandle texture, RenderBufferTarget target, u32 slot) override;
        void UseSampler(TextureHandle texture, RenderBufferTarget target, u32 slot) override;

        void UseShader(ShaderHandle shader) override;

        void TransferResourceData(RenderBufferHandle inputBuffer, RenderBufferHandle outputBuffer) override;
        void TransferResourceData(TextureHandle inputTexture, TextureHandle outputTexture) override;

        void Frame(u32 interval) override;
        void HandlePreFrame() override;

        void DrawIndexed(u32 indexCount, u32 startIndex, u32 baseVertex) override;
        void Dispatch(u32 thrX, u32 thrY, u32 thrZ) override;

        static D3D12Renderer *&Get();

    private:
        bool CreateDevice();
        bool CreateCommandQueue();
        bool CreateSwapChain(PlatformWindow *pWindow, u32 width, u32 height);
        bool CreateCommandAllocator();

    public:
        ID3D12Device *GetDevice() const
        {
            return m_pDevice;
        }

        IDXGISwapChain3 *GetSwapChain() const
        {
            return m_pSwapChain;
        }

    private:
        //* Main API *//
        ID3D12Device *m_pDevice = nullptr;
#ifdef _DEBUG
        ID3D12Debug1 *m_pDebug = nullptr;
        ID3D12DebugDevice *m_pDebugDevice = nullptr;
#endif
        IDXGIFactory4 *m_pFactory = nullptr;
        IDXGIAdapter1 *m_pAdapter = nullptr;

        IDXGISwapChain3 *m_pSwapChain = nullptr;
        DXGI_SWAP_CHAIN_DESC1 m_SwapChainDesc;
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC m_SwapChainFSD = {};
        u32 m_BackBufferIndex = 0;

        ID3D12CommandQueue *m_pCommandQueue = nullptr;
        ID3D12CommandAllocator *m_pCommandAllocator = nullptr;

        ID3D12Fence *m_pFence = nullptr;
        ID3D12GraphicsCommandList *m_pCommandList = nullptr;

        //* View handles *//
        TextureHandle m_DepthTexture = nullptr;

        //* Current API states *//
        D3D12_BLEND_DESC m_BlendDesc = {};
        D3D12_RASTERIZER_DESC m_RasterizerDesc = {};
        D3D12_DEPTH_STENCIL_VIEW_DESC m_DepthStencilViewDesc = {};
        D3D12_DEPTH_STENCIL_DESC m_DepthStencilDesc = {};

        D3D12RenderTargetManager m_TargetManager;

        bool m_IsContextReady = false;
    };

#define DX12Renderer D3D12Renderer::Get()

}  // namespace Lorr

#endif