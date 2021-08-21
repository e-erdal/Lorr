//
// Created on August 20th 2021 by e-erdal.
//

#pragma once

#include "Core/Graphics/API/D3D11/D3D11API.hh"
#include "Core/Graphics/Texture/Texture2D.hh"

namespace Lorr
{
    inline DXGI_FORMAT ToDXFormat(eTextureFormat format)
    {
        switch (format)
        {
        case TEXTURE_FORMAT_RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
        default: return DXGI_FORMAT_UNKNOWN;
        }
    }

    class D3D11Texture2D: Texture2D
    {
    public:
        void Init(Identifier const &Ident, TEXTURE2D_DESC *pTextureD, TEXTURE2D_DESC_SUBRESC *pTextureSRES = 0) override;

    private:
        ID3D11Texture2D *m_Handle = 0;
        ID3D11ShaderResourceView *m_ShaderResource = 0;
        ID3D11SamplerState *m_SamplerState = 0;
        ID3D11RenderTargetView *m_RenderTarget = 0;
    };
}  // namespace Lorr