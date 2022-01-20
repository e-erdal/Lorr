//
// Created on Sunday 2nd January 2022 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/Type.hh"
#include "Engine/Core/Window/PlatformWindow.hh"

namespace lr
{
    class D3D11SwapChain
    {
    public:
        D3D11SwapChain() = default;

        void Init(ID3D11Device *pDevice, PlatformWindow *pWindow, SwapChainFlags flags);
        void Resize(u32 width, u32 height);
        void Present();

        ID3D11Texture2D *GetBackBuffer();

    private:
        IDXGISwapChain *m_pHandle = nullptr;
        DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;

        bool m_vSync = false;
        bool m_Tearing = false;
    };

}  // namespace lr