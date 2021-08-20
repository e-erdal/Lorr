//
// Created on July 6th 2021 by e-erdal.
//

#pragma once

#include "Core/Graphics/API/RendererAPI.hh"

#include <d3d11.h>

namespace Lorr
{
    class D3D11API: public RendererAPI
    {
    public:
        bool Init(PlatformWindow *pWindow, int width, int height) override;

        void ChangeResolution(uint32_t width, uint32_t height) override;
        void SetViewport(int width, int height, float farZ, float nearZ) override;
        void SetClearColor(const glm::vec4 &color) override;
        void Frame(uint32_t interval) override;

        static D3D11API *&Get();

    private:
        ID3D11Texture2D *CreateDepthStencilBuffer(uint32_t width, uint32_t height);

        bool CreateDevice(PlatformWindow *pWindow, int width, int height);

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
        ID3D11Device *m_pDevice;
        ID3D11DeviceContext *m_pDeviceContext;
        IDXGISwapChain *m_pSwapChain;

        ID3D11BlendState *m_pBlendState;
        ID3D11RenderTargetView *m_pRenderTargetView;
        ID3D11DepthStencilView *m_pDepthStencilView;
        ID3D11Texture2D *m_pDepthStencilBuffer;

        ID3D11DepthStencilState *m_pDepthStencilState;
        ID3D11RasterizerState *m_pRasterizerState;

        bool m_NeedToPresent = true;
        bool m_IsContextReady = false;
    };
}  // namespace Lorr