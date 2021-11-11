//
// Created on Tuesday 12th October 2021 by e-erdal
//

#pragma once

#if LR_BACKEND_D3D11

#include "Engine/Graphics/Common/ITexture.hh"
#include "Engine/Graphics/Common/IRenderer.hh"
#include "Engine/Graphics/D3D11/D3D11Renderer.hh"

namespace Lorr
{
    class D3D11Texture : public ITexture
    {
    public:
        void Init(const Identifier &ident, TextureDesc *pDesc, TextureData *pData) override;
        void Use() override;
        void *GetHandle() override;
        void Delete() override;

        void GenerateMips() override;
        void Map();

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

    private:
        void CreateTexture2D(TextureData *pData);
        void CreateDepthTexture();
        void CreateRenderTarget();
        void CreateRWTexture();

        void CreateShaderResource();
        void CreateSampler();

    private:
        ID3D11Texture2D *m_pHandle = 0;
        D3D11_TEXTURE2D_DESC m_TextureDesc = {};

        ID3D11SamplerState *m_pSamplerState = 0;
        ID3D11ShaderResourceView *m_pShaderResource = 0;

        ID3D11UnorderedAccessView *m_pUAV = 0;
        ID3D11RenderTargetView *m_pRenderTarget = 0;
    };
}  // namespace Lorr

#endif