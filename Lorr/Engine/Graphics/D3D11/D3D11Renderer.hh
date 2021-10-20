
//
// Created on July 6th 2021 by e-erdal.
//

#pragma once

#include <d3d11.h>

#include "Engine/Graphics/Common/IRenderer.hh"
#include "Engine/Graphics/Common/IShader.hh"
#include "Engine/Graphics/Common/ITexture.hh"

#include "D3D11RendererStateManager.hh"
#include "D3D11RenderTargetManager.hh"

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
        void SetScissor(const glm::vec4 &lrtb) override;
        void SetDepthFunc(D3D::DepthFunc func, bool depthEnabled) override;
        void SetCulling(D3D::Cull cull, bool counterClockwise) override;
        void SetBlend(bool enableBlending, bool alphaCoverage) override;
        void CreateTarget(const Identifier &ident, uint32_t width, uint32_t height, TextureHandle texture = 0) override;
        void SetCurrentTarget(const Identifier &ident) override;
        TextureHandle GetTargetTexture(const Identifier &ident) override;

        void Frame(uint32_t interval) override;
        void HandlePreFrame() override;

        void DrawIndexed(uint32_t indexCount, uint32_t startIndex, uint32_t baseVertex) override;

        static D3D11Renderer *&Get();

    private:
        bool CreateDevice();
        bool CreateSwapChain(PlatformWindow *pWindow, uint32_t width, uint32_t height);
        bool CreateBackBuffer();
        bool CreateDepthTexture(uint32_t width, uint32_t height);
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
            return m_pDeviceContext;
        }

    private:
        //* Main API *//
        ID3D11Device *m_pDevice = 0;
        ID3D11DeviceContext *m_pDeviceContext = 0;
        IDXGISwapChain *m_pSwapChain = 0;
        DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;

        //* API Managers *//
        D3D11RendererStateManager m_StateManager;
        D3D11RenderTargetManager m_TargetManager;

        //* View handles *//
        TextureHandle m_DepthTexture = 0;

        //* Current API states *//
        D3D11_BLEND_DESC m_BlendDesc = {};
        D3D11_RASTERIZER_DESC m_RasterizerDesc = {};
        D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilViewDesc = {};
        D3D11_DEPTH_STENCIL_DESC m_DepthStencilDesc = {};

        ID3D11BlendState *m_pBlendState = 0;
        ID3D11RasterizerState *m_pRasterizerState = 0;
        ID3D11DepthStencilView *m_pDepthStencilView = 0;
        ID3D11DepthStencilState *m_pDepthStencilState = 0;

        bool m_NeedToPresent = true;
        bool m_IsContextReady = false;
    };

}  // namespace Lorr