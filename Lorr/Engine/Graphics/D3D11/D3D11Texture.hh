//
// Created on Tuesday 12th October 2021 by e-erdal
//

#pragma once

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

    private:
        ID3D11Texture2D *m_pHandle = 0;
        ID3D11ShaderResourceView *m_pShaderResource = 0;
        ID3D11SamplerState *m_pSamplerState = 0;
        ID3D11RenderTargetView *m_pRenderTarget = 0;
    };
}  // namespace Lorr