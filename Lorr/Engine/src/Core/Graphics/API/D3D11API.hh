//
// Created on July 6th 2021 by e-erdal.
//

#pragma once

#include "Core/Graphics/Window.hh"

#include <d3d11.h>

namespace Lorr
{
    class D3D11API
    {
    public:
        ~D3D11API(){};

        bool Init( Window *pWindow, int iWidth, int iHeight );

        void SetViewport( int iWidth, int iHeight, float fFar, float fNear );
        void SetClearColor( const glm::vec4 &v4Color );

        void Frame( uint32_t uInterval );

        bool CreateIndexBuffer( uint32_t *puIndexes, uint32_t uCount, ID3D11Buffer **pBufferOut );
        void CreateTexture2D( ID3D11Texture2D **ppTarget, int iWidth, int iHeight, DXGI_FORMAT eFormat );
        bool CreateDevice( Window *pWindow, int iWidth, int iHeight );
        bool CreateSwapChain();

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
        ID3D11Device *m_pDevice;
        ID3D11DeviceContext *m_pDeviceContext;
        IDXGISwapChain *m_pSwapChain;

        ID3D11BlendState *m_pBlendState;
        ID3D11RenderTargetView *m_pRenderTargetView;
        ID3D11DepthStencilView *m_pDepthStencilView;
        ID3D11Texture2D *m_pDepthStencilBuffer;

        ID3D11DepthStencilState *m_pDepthStencilState;
        ID3D11RasterizerState *m_pRasterizerState;
    };
}  // namespace Lorr