#ifdef LR_BACKEND_D3D12
#include "D3D12SwapChain.hh"

namespace lr
{
    void D3D12SwapChain::Init(ID3D12Device *pDevice, IDXGIFactory4 *pFactory, ID3D12CommandQueue *pCommandQueue, PlatformWindow *pWindow,
                              SwapChainFlags flags)
    {
        m_pDevice = pDevice;

        (flags & SwapChainFlags::vSync) ? m_vSync = true : 0;
        (flags & SwapChainFlags::AllowTearing) ? m_Tearing = true : 0;

        u32 bufferCount = m_UsingBufferCount = (flags & SwapChainFlags::TripleBuffering) ? 3 : 2;

        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = bufferCount;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        HRESULT hr;
        HRCall(pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_pBufferHeap)), "Failed to create Swap Chain RTV descriptor.");

        m_IncrementSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        m_SwapChainDesc = {};

        m_SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        m_SwapChainDesc.Width = pWindow->GetWidth();
        m_SwapChainDesc.Height = pWindow->GetHeight();

        m_SwapChainDesc.SampleDesc.Count = 1;
        m_SwapChainDesc.SampleDesc.Quality = 0;

        m_SwapChainDesc.BufferCount = bufferCount;
        m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        m_SwapChainFSD = {};
        m_SwapChainFSD.Windowed = true;

        HRCall(pFactory->CreateSwapChainForHwnd(pCommandQueue, (HWND)pWindow->GetHandle(), &m_SwapChainDesc, &m_SwapChainFSD, nullptr,
                                                (IDXGISwapChain1 **)&m_pHandle),
               "Failed to create D3D12 Swap Chain.");
    }

    void D3D12SwapChain::Present()
    {
        m_pHandle->Present(m_vSync, m_Tearing ? DXGI_PRESENT_ALLOW_TEARING : 0);

        m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % m_UsingBufferCount;
    }

    void D3D12SwapChain::Resize(u32 width, u32 height)
    {
        m_CurrentBufferIndex = 0;

        m_pHandle->ResizeBuffers(m_UsingBufferCount, width, height, DXGI_FORMAT_UNKNOWN,
                                 DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
        RecreateBuffers();
    }

    void D3D12SwapChain::RecreateBuffers()
    {
        D3D12_CPU_DESCRIPTOR_HANDLE descriptor(m_pBufferHeap->GetCPUDescriptorHandleForHeapStart());

        for (u32 i = 0; i < m_UsingBufferCount; i++)
        {
            SAFE_RELEASE(m_Buffers[i]);

            m_pHandle->GetBuffer(i, IID_PPV_ARGS(&m_Buffers[i]));
            m_pDevice->CreateRenderTargetView(m_Buffers[i], nullptr, descriptor);
            descriptor.ptr += m_IncrementSize;
        }
    }

}  // namespace lr
#endif