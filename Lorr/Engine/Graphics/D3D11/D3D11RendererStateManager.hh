//
// Created on Monday 18th October 2021 by e-erdal
// A tool that will help renderer to handle/cache states
//

#pragma once

#if LR_BACKEND_D3D11

#include <d3d11.h>

#include "Engine/Utils/Hash.hh"

HASH_INTERFACE(D3D11_RASTERIZER_DESC);
HASH_INTERFACE(D3D11_DEPTH_STENCIL_DESC);
HASH_INTERFACE(D3D11_BLEND_DESC);

namespace lr
{
    class D3D11RendererStateManager
    {
    public:
        void SetDevice(ID3D11Device *pDevice);

        ID3D11RasterizerState *Get(D3D11_RASTERIZER_DESC &desc);
        ID3D11DepthStencilState *Get(D3D11_DEPTH_STENCIL_DESC &desc);
        ID3D11BlendState *Get(D3D11_BLEND_DESC &desc);

    private:
        std::unordered_map<D3D11_RASTERIZER_DESC, ID3D11RasterizerState *> m_RasterizerStack;
        std::unordered_map<D3D11_DEPTH_STENCIL_DESC, ID3D11DepthStencilState *> m_DepthStack;
        std::unordered_map<D3D11_BLEND_DESC, ID3D11BlendState *> m_BlendStack;

        ID3D11Device *m_pDevice = 0;
    };

}  // namespace lr

// std::unordered_map operator support
OPR_INTERFACE_CMP(D3D11_RASTERIZER_DESC);
OPR_INTERFACE_CMP(D3D11_DEPTH_STENCIL_DESC);
OPR_INTERFACE_CMP(D3D11_BLEND_DESC);

#endif