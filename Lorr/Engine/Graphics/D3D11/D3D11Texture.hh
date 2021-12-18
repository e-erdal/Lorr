//
// Created on Tuesday 12th October 2021 by e-erdal
//

#pragma once

#if LR_BACKEND_D3D11

#include "Engine/Graphics/Common/BaseTexture.hh"
#include "Engine/Graphics/Common/BaseRenderer.hh"

#include "Engine/Graphics/D3D11/D3D11Renderer.hh"

namespace lr
{
    class D3D11Texture : public BaseTexture
    {
    public:
        void Init(const Identifier &ident, TextureDesc *pDesc, TextureData *pData) override;
        void Use() override;
        void *GetHandle() override;
        void Delete() override;

        void GenerateMips() override;
        void Map(D3D11Texture *pTarget);

        ~D3D11Texture();

    public:
        ID3D11ShaderResourceView *GetShaderResource()
        {
            return m_pShaderResource;
        }

        ID3D11UnorderedAccessView *GetUAV()
        {
            return m_pUAV;
        }

        ID3D11RenderTargetView *GetRenderTarget()
        {
            return m_pRenderTarget;
        }

        ID3D11SamplerState *GetSampler()
        {
            return m_pSamplerState;
        }

        ID3D11DepthStencilView *GetDepthStencil()
        {
            return m_pDepthStencil;
        }

    private:
        void CreateTexture2D(TextureData *pData);
        void CreateDepthTexture();
        void CreateRenderTarget();
        void CreateRWTexture();
        void CreateStagingTexture();

        void CreateShaderResource();
        void CreateSampler();

    private:
        ID3D11Texture2D *m_pHandle = 0;
        D3D11_TEXTURE2D_DESC m_TextureDesc = {};

        ID3D11SamplerState *m_pSamplerState = 0;
        ID3D11ShaderResourceView *m_pShaderResource = 0;

        ID3D11UnorderedAccessView *m_pUAV = 0;
        ID3D11RenderTargetView *m_pRenderTarget = 0;

        ID3D11DepthStencilView *m_pDepthStencil = 0;
    };
}  // namespace lr

#endif