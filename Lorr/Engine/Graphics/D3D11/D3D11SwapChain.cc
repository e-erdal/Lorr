#include "D3D11SwapChain.hh"

#include <bx/string.h>

namespace lr
{
    void D3D11SwapChain::Init(ID3D11Device *pDevice, PlatformWindow *pWindow, SwapChainFlags flags)
    {
        (flags & SwapChainFlags::vSync) ? m_vSync = true : 0;
        (flags & SwapChainFlags::AllowTearing) ? m_Tearing = true : 0;

        m_SwapChainDesc = {};

        // Buffer description
        m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        m_SwapChainDesc.BufferDesc.Width = pWindow->GetWidth();
        m_SwapChainDesc.BufferDesc.Height = pWindow->GetHeight();
        m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = pWindow->GetDisplay(pWindow->GetUsingMonitor())->RefreshRate;
        m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        m_SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        m_SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        m_SwapChainDesc.SampleDesc.Count = 1;
        m_SwapChainDesc.SampleDesc.Quality = 0;

        m_SwapChainDesc.BufferCount = 1;
        m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        m_SwapChainDesc.OutputWindow = (HWND)pWindow->GetHandle();
        m_SwapChainDesc.Windowed = !pWindow->IsFullscreen();

        IDXGIDevice *pDXGIDevice = 0;
        IDXGIAdapter *pDXGIAdapter = 0;
        IDXGIFactory *pDXGIFactory = 0;
        IDXGIOutput *pDXGIOutput = 0;
        pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
        pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);  // get interface factory from our device
        pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pDXGIFactory);

        HRESULT hr;
        HRCall(pDXGIFactory->CreateSwapChain(pDevice, &m_SwapChainDesc, &m_pHandle), "Failed to create D3D11 swapchain!");

        DXGI_ADAPTER_DESC adapterDesc = {};
        pDXGIAdapter->GetDesc(&adapterDesc);

        char driverMame[BX_COUNTOF(adapterDesc.Description)];
        wcstombs(driverMame, adapterDesc.Description, BX_COUNTOF(adapterDesc.Description));

        char dedicatedVideo[16];
        bx::prettify(dedicatedVideo, BX_COUNTOF(dedicatedVideo), adapterDesc.DedicatedVideoMemory);

        LOG_INFO("DXGI Desc: %s (%d, %d).", driverMame, adapterDesc.VendorId, adapterDesc.DeviceId);
        LOG_INFO("GPU Dedicated Memory: %s.", dedicatedVideo);

        SAFE_RELEASE(pDXGIOutput);
        SAFE_RELEASE(pDXGIDevice);
        SAFE_RELEASE(pDXGIAdapter);
        SAFE_RELEASE(pDXGIFactory);

        /// Check if we are in fullscreen
        if (pWindow->IsFullscreen())
        {
            LOG_TRACE("Going into fullscreen. (swapchain)");
            m_pHandle->SetFullscreenState(true, 0);
        }
    }

    void D3D11SwapChain::Resize(u32 width, u32 height)
    {
        HRESULT hr;
        HRCall(m_pHandle->ResizeBuffers(0, 0, 0, m_SwapChainDesc.BufferDesc.Format, 0), "Failed to resize D3D11 SwapChain!");
    }

    void D3D11SwapChain::Present()
    {
        m_pHandle->Present(m_vSync, (m_Tearing) ? DXGI_PRESENT_ALLOW_TEARING : 0);
    }

    ID3D11Texture2D *D3D11SwapChain::GetBackBuffer()
    {
        ID3D11Texture2D *pBackBuffer = nullptr;

        HRESULT hr;
        HRCallRet(m_pHandle->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)), "Failed to get D3D11 SwapChain BackBuffer!", nullptr);

        return pBackBuffer;
    }

}  // namespace lr