//
// Created on Monday 13th December 2021 by e-erdal
//

#pragma once
#ifdef LR_BACKEND_D3D12

#include "Engine/Graphics/D3D/D3DType.hh"
#include "Engine/Core/Window/PlatformWindow.hh"

namespace lr
{
    class D3D12SwapChain
    {
    public:
        D3D12SwapChain() = default;

        void Init(ID3D12Device *pDevice, IDXGIFactory4 *pFactory, ID3D12CommandQueue *pCommandQueue, PlatformWindow *pWindow, SwapChainFlags flags);
        void Resize(u32 width, u32 height);
        void Present();

        ID3D11Texture2D *GetBackBuffer();

    private:
        void RecreateBuffers();

    private:
        IDXGISwapChain3 *m_pHandle = nullptr;
        DXGI_SWAP_CHAIN_DESC1 m_SwapChainDesc;
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC m_SwapChainFSD = {};

        ID3D12Device *m_pDevice = nullptr;

        ID3D12DescriptorHeap *m_pBufferHeap = nullptr;
        ID3D12Resource *m_Buffers[3];

        /// Ring buffer
        u32 m_CurrentBufferIndex = 0;
        u32 m_UsingBufferCount = 0;
        u32 m_IncrementSize = 0;

        bool m_vSync = false;
        bool m_Tearing = false;
    };

}  // namespace lr

#endif