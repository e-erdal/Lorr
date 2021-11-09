
//
// Created on July 6th 2021 by e-erdal.
//

#pragma once

#if LR_BACKEND_D3D11

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

        void UseShader(ShaderHandle shader) override;

        void TransferResourceData(RenderBufferHandle inputBuffer, RenderBufferHandle outputBuffer) override;

        void Frame(u32 interval) override;
        void HandlePreFrame() override;

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
        ID3D11Device *m_pDevice = 0;
        ID3D11DeviceContext *m_pContext = 0;
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

#endif