//
// Created on Wednesday 20th October 2021 by e-erdal
// AKA framebuffers
//

#pragma once

#include "Engine/Graphics/Common/ITexture.hh"

#include <d3d11.h>

namespace Lorr
{
    class D3D11RenderTargetManager
    {
    public:
        D3D11RenderTargetManager() = default;

        void Init(ID3D11Device *pDevice);
        void ClearAll(ID3D11DeviceContext *pContext);
        void ReInitAll(ID3D11DepthStencilView *pDepthStencil);

        ID3D11RenderTargetView *Create(const Identifier &ident, uint32_t width, uint32_t height, TextureHandle pTargetTexture);
        ID3D11RenderTargetView *Create(const Identifier &ident, ID3D11Texture2D *pTexture);

        ID3D11RenderTargetView *GetView(const Identifier &ident);
        TextureHandle GetTexture(const Identifier &ident);

        void Release(const Identifier &ident);

    private:
        struct RenderTarget
        {
            ID3D11RenderTargetView *pView = 0;
            TextureData textureData;
            TextureHandle Texture = 0;
        };

        std::unordered_map<Identifier, RenderTarget> m_Targets;

        ID3D11Device *m_pDevice = 0;
    };

}  // namespace Lorr