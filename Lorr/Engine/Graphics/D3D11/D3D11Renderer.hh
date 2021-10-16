
//
// Created on July 6th 2021 by e-erdal.
//

#pragma once

#include "Engine/Graphics/Common/IRenderer.hh"
#include "Engine/Graphics/Common/IShader.hh"
#include "Engine/Graphics/Common/ITexture.hh"

#include <d3d11.h>

namespace Lorr
{
    class D3D11Texture;
    class D3D11Renderer : public IRenderer
    {
    public:
        bool Init(PlatformWindow *pWindow, uint32_t width, uint32_t height) override;

        void ChangeResolution(uint32_t width, uint32_t height) override;
        void SetViewport(uint32_t width, uint32_t height, float farZ, float nearZ) override;
        void SetClearColor(const glm::vec4 &color) override;
        void Frame(uint32_t interval) override;
        void HandlePreFrame() override;

        void DrawIndexed(uint32_t indexCount) override;

        static D3D11Renderer *&Get();

    private:
        bool CreateDevice();
        bool CreateSwapChain(PlatformWindow *pWindow, uint32_t width, uint32_t height);
        bool CreateBackBuffer();
        bool CreateDepthStencil(uint32_t width, uint32_t height);
        bool CreateRasterizer();

    public:
        ID3D11Device *GetDevice() const
        {
            return m_pDevice;
        }

        ID3D11DeviceContext *GetDeviceContext() const
        {
            return m_pDeviceContext;
        }

        ID3D11RasterizerState *GetRasterizerState() const
        {
            return m_pRasterizerState;
        }

        ID3D11DepthStencilState *GetDepthStencilState() const
        {
            return m_pDepthStencilState;
        }

        ID3D11DepthStencilView *GetDepthStencilView() const
        {
            return m_pDepthStencilView;
        }

        ID3D11RenderTargetView *GetRenderTargetView() const
        {
            return m_pRenderTargetView;
        }

    private:
        //* Main API *//
        ID3D11Device *m_pDevice = 0;
        ID3D11DeviceContext *m_pDeviceContext = 0;
        IDXGISwapChain *m_pSwapChain = 0;
        DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;

        //* View handles *//
        ID3D11RenderTargetView *m_pRenderTargetView = 0;
        ID3D11DepthStencilView *m_pDepthStencilView = 0;
        TextureHandle m_DepthTexture = 0;
        D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilViewDesc;

        //* API states *//
        ID3D11BlendState *m_pBlendState = 0;
        ID3D11DepthStencilState *m_pDepthStencilState = 0;
        D3D11_DEPTH_STENCIL_DESC m_DepthStencilDesc;
        ID3D11RasterizerState *m_pRasterizerState = 0;
        D3D11_RASTERIZER_DESC m_RasterizerDesc;

        bool m_NeedToPresent = true;
        bool m_IsContextReady = false;
    };

}  // namespace Lorr