#ifdef LR_BACKEND_D3D12
#include "D3D12SwapChain.hh"

namespace Lorr
{
    void D3D12SwapChain::Init(ID3D12Device *pDevice, IDXGIFactory4 *pFactory, BaseWindow *pWindow, DXGI_FORMAT bufferingFormat, bool tripleBuffering)
    {
        m_pDevice = pDevice;
        u32 bufferCount = tripleBuffering ? 3 : 2;
        m_pRingBuffer = new bx::RingBufferControl(bufferCount);

        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = tripleBuffering;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pFrameBufferHeap));

        m_SwapChainDesc = {};

        m_SwapChainDesc.Format = bufferingFormat;
        m_SwapChainDesc.Width = pWindow->GetWidth();
        m_SwapChainDesc.Height = pWindow->GetHeight();

        m_SwapChainDesc.SampleDesc.Count = 1;
        m_SwapChainDesc.SampleDesc.Quality = 0;

        m_SwapChainDesc.BufferCount = bufferCount;
        m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        m_SwapChainFSD = {};
        m_SwapChainFSD.RefreshRate.Numerator = pWindow->GetDisplay(pWindow->GetUsingMonitor())->RefreshRate;
        m_SwapChainFSD.RefreshRate.Denominator = 1;
        m_SwapChainFSD.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        m_SwapChainFSD.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        m_SwapChainFSD.Windowed = !pWindow->IsFullscreen();

        pFactory->CreateSwapChainForHwnd(pDevice, (HWND)pWindow->GetHandle(), &m_SwapChainDesc, &m_SwapChainFSD, nullptr, (IDXGISwapChain1 **)&m_pSwapChain);

        m_BackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

        RecreateBuffers();
    }

    void D3D12SwapChain::Present(bool vSync)
    {
        m_pSwapChain->Present(vSync, vSync ? 0 : DXGI_PRESENT_ALLOW_TEARING);

        m_pRingBuffer->consume(1);
    }

    void D3D12SwapChain::Resize(u32 width, u32 height)
    {
        m_pSwapChain->ResizeBuffers(m_pRingBuffer->m_size, width, height, DXGI_FORMAT_UNKNOWN,
                                    DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

        m_pRingBuffer->reset();
        RecreateBuffers();
    }

    void D3D12SwapChain::RecreateBuffers()
    {
        u32 descriptorHeapSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        for (u32 i = 0; i < m_pRingBuffer->m_size; i++)
        {
            SAFE_RELEASE(m_FrameBuffers[i]);

            CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(m_pFrameBufferHeap->GetCPUDescriptorHandleForHeapStart());

            m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_FrameBuffers[i]));
            m_pDevice->CreateRenderTargetView(m_FrameBuffers[i], nullptr, descriptorHandle);
            descriptorHandle.Offset(1, descriptorHeapSize);
        }
    }

}  // namespace Lorr
#endif