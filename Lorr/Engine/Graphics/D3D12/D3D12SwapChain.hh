//
// Created on Monday 13th December 2021 by e-erdal
//

#pragma once
#ifdef LR_BACKEND_D3D12

#include <bx/ringbuffer.h>

#include "Engine/Core/Window/BaseWindow.hh"

namespace Lorr
{
    class D3D12SwapChain
    {
    public:
        D3D12SwapChain() = default;

        void Init(ID3D12Device *pDevice, IDXGIFactory4 *pFactory, BaseWindow *pWindow, DXGI_FORMAT bufferingFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
                  bool tripleBuffering = false);

        void Present(bool vSync);
        void Resize(u32 width, u32 height);

    public:
        IDXGISwapChain3 *GetHandle()
        {
            return m_pSwapChain;
        }

        u32 GetBackBufferIndex()
        {
            return m_BackBufferIndex;
        }

    private:
        void RecreateBuffers();

    private:
        ID3D12Device *m_pDevice = nullptr;

        IDXGISwapChain3 *m_pSwapChain = nullptr;
        DXGI_SWAP_CHAIN_DESC1 m_SwapChainDesc;
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC m_SwapChainFSD = {};

        u32 m_BackBufferIndex = 0;
        ID3D12DescriptorHeap *m_pFrameBufferHeap = nullptr;
        std::array<ID3D12Resource *, 3> m_FrameBuffers;
        bx::RingBufferControl *m_pRingBuffer;
    };

}  // namespace Lorr

#endif