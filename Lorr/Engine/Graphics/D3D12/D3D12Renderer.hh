//
// Created on Friday 22nd October 2021 by e-erdal
//

#pragma once

#if LR_BACKEND_D3D12

    #include <d3d12.h>
    #include <dxgi1_4.h>

    #include "Engine/Graphics/Common/BaseRenderer.hh"
    #include "Engine/Graphics/Common/BaseShader.hh"
    #include "Engine/Graphics/Common/BaseTexture.hh"

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

        void Frame(u32 interval) override;
        void HandlePreFrame() override;

        void DrawIndexed(u32 indexCount, u32 startIndex, u32 baseVertex) override;

        static D3D12Renderer *&Get();

    private:
        bool CreateDevice();
        bool CreateSwapChain(PlatformWindow *pWindow, u32 width, u32 height);
        bool CreateBackBuffer();
        bool CreateDepthTexture(u32 width, u32 height);
        bool CreateDepthStencil();
        bool CreateBlendState();
        bool CreateRasterizer();

    public:
        ID3D12Device *GetDevice() const
        {
            return m_pDevice;
        }

    private:
        //* Main API *//
        ID3D12Device *m_pDevice = 0;
    #ifdef _DEBUG
        ID3D12Debug1 *m_pDebug = 0;
        ID3D12DebugDevice *m_pDebugDevice = 0;
    #endif
        IDXGIFactory4 *m_pFactory = 0;
        IDXGIAdapter1 *m_pAdapter = 0;
        IDXGISwapChain3 *m_pSwapChain = 0;
        DXGI_SWAP_CHAIN_DESC1 m_SwapChainDesc;
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC m_SwapChainFSD = {};
        ID3D12Fence *m_pFence = 0;

        // d3d11 device context
        ID3D12CommandQueue *m_pCommandQueue = 0;
        ID3D12CommandAllocator *m_pCommandAllocator = 0;
        ID3D12GraphicsCommandList *m_pCommandList = 0;

        //* View handles *//
        TextureHandle m_DepthTexture = 0;

        //* Current API states *//
        D3D12_BLEND_DESC m_BlendDesc = {};
        D3D12_RASTERIZER_DESC m_RasterizerDesc = {};
        D3D12_DEPTH_STENCIL_VIEW_DESC m_DepthStencilViewDesc = {};
        D3D12_DEPTH_STENCIL_DESC m_DepthStencilDesc = {};

        bool m_IsContextReady = false;
    };

}  // namespace Lorr

#endif