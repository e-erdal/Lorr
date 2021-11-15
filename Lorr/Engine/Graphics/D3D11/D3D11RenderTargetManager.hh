//
// Created on Wednesday 20th October 2021 by e-erdal
// AKA framebuffers
//

#pragma once

#if LR_BACKEND_D3D11

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
        void Resize(u32 width, u32 height);

        ID3D11RenderTargetView *Create(const Identifier &ident, u32 width, u32 height, TextureHandle pTargetTexture, u32 mipLevels = 1);
        ID3D11RenderTargetView *Create(const Identifier &ident, ID3D11Texture2D *pTexture, bool isBackbuffer);

        ID3D11RenderTargetView *GetView(const Identifier &ident);
        TextureHandle GetTexture(const Identifier &ident);

        void Release(const Identifier &ident);

    private:
        struct RenderTarget
        {
            ID3D11RenderTargetView *pView = 0;
            TextureHandle ViewTexture = 0;
            TextureDesc ViewTextureDesc;
            TextureData ViewTextureData;

            bool IsBackbuffer = false;

            ~RenderTarget()
            {
                pView->Release();
                pView = 0;
                
                if (!IsBackbuffer)
                {
                    delete ViewTexture;
                    ViewTexture = 0;
                }
            }
        };

        std::unordered_map<Identifier, RenderTarget *> m_Targets;

        ID3D11Device *m_pDevice = 0;
    };

}  // namespace Lorr

#endif