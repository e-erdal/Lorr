#if LR_BACKEND_D3D12
    #include "D3D12Renderer.hh"

// #include "Engine/Graphics/D3D12/D3D12Texture.hh"

static Lorr::D3D12Renderer *g_D3D12Renderer;

namespace Lorr
{
    bool D3D12Renderer::Init(PlatformWindow *pWindow, u32 width, u32 height)
    {
        ZoneScoped;

        g_D3D12Renderer = this;
        m_CurrentAPI = RendererType::D3D12;

        if (!CreateDevice()) return false;

        if (!pWindow)
        {
            LOG_WARN("Console application mode, only initializing D3D12 device.");
            return true;
        }

        if (!CreateSwapChain(pWindow, width, height)) return false;
        if (!CreateBackBuffer()) return false;
        if (!CreateDepthTexture(width, height)) return false;
        if (!CreateDepthStencil()) return false;
        if (!CreateBlendState()) return false;
        if (!CreateRasterizer()) return false;

        SetViewport(width, height, 1.f, 0.0f);

        m_IsContextReady = true;

        LOG_INFO("Successfully initialized D3D12 device.");

        /*TextureDesc desc;
        constexpr u32 whiteColor = 0xffffffff;
        TextureData data;
        data.Width = 1;
        data.Height = 1;
        data.DataSize = sizeof(u32);
        data.Data = (u8 *)&whiteColor;

        m_PlaceholderTexture = Texture::Create("batcher://placeholder", &desc, &data);
        */

        InitParent();
        
        return true;
    }

    void D3D12Renderer::SetClearColor()
    {
        ZoneScoped;
        if (!m_IsContextReady) return;
    }

    void D3D12Renderer::Frame(u32 uInterval)
    {
        ZoneScoped;

        if (!m_IsContextReady) return;

        m_pSwapChain->Present(uInterval, 0);
    }

    void D3D12Renderer::SetViewport(u32 width, u32 height, float fFar, float fNear)
    {
        ZoneScoped;

        if (!m_IsContextReady) return;

        D3D12_VIEWPORT vp = {};

        vp.Width = (float)width;
        vp.Height = (float)height;
        vp.MaxDepth = fFar;
        vp.MinDepth = fNear;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;

        m_pCommandList->RSSetViewports(1, &vp);
    }

    bool D3D12Renderer::CreateDevice()
    {
        ZoneScoped;

        LOG_INFO("Initializing D3D12 device...");

        HRESULT hr;
        u32 flags = 0;

    #ifdef _DEBUG
        flags |= DXGI_CREATE_FACTORY_DEBUG;

        ID3D12Debug *pDebug = 0;
        D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebug));
        pDebug->QueryInterface(IID_PPV_ARGS(&m_pDebug));

        m_pDebug->EnableDebugLayer();
        m_pDebug->SetEnableGPUBasedValidation(true);

        SAFE_RELEASE(pDebug);
    #endif

        if (FAILED(hr = CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_pFactory))))
        {
            LOG_ERROR("Failed to create D3D12 factory.");
            return false;
        }

        for (u32 i = 0; m_pFactory->EnumAdapters1(i, &m_pAdapter) != DXGI_ERROR_NOT_FOUND; i++)
        {
            DXGI_ADAPTER_DESC1 adapterDesc = {};
            m_pAdapter->GetDesc1(&adapterDesc);

            // Note: This does not have something to do with hardware or software driver.
            if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

            if (SUCCEEDED(hr = D3D12CreateDevice(m_pAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_pDevice))))
            {
                break;
            }

            SAFE_RELEASE(m_pAdapter);
        }

        if (!m_pDevice || FAILED(hr))
        {
            LOG_ERROR("Failed to create D3D12 device!");
            return false;
        }

    #ifdef _DEBUG
        m_pDebug->QueryInterface(&m_pDebugDevice);
    #endif

        D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
        commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        if (FAILED(hr = m_pDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_pCommandQueue))))
        {
            LOG_ERROR("Failed to create D3D12 command queue!");
            return false;
        }

        if (FAILED(hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator))))
        {
            LOG_ERROR("Failed to create D3D12 command allocator!");
            return false;
        }

        if (FAILED(hr = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence))))
        {
            LOG_ERROR("Failed to create D3D12 fence!");
            return false;
        }

        return true;
    }

    bool D3D12Renderer::CreateSwapChain(PlatformWindow *pWindow, u32 width, u32 height)
    {
        HRESULT hr;
        m_SwapChainDesc = {};

        pWindow->OnResolutionChanged.connect<&D3D12Renderer::ChangeResolution>(this);

        m_SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        m_SwapChainDesc.Width = width;
        m_SwapChainDesc.Height = height;

        m_SwapChainDesc.SampleDesc.Count = 1;
        m_SwapChainDesc.SampleDesc.Quality = 0;

        m_SwapChainDesc.BufferCount = 1;
        m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        m_SwapChainFSD = {};
        m_SwapChainFSD.RefreshRate.Numerator = 60;
        m_SwapChainFSD.RefreshRate.Denominator = 1;
        m_SwapChainFSD.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        m_SwapChainFSD.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        m_SwapChainFSD.Windowed = !pWindow->IsFullscreen();

        if (FAILED(hr = m_pFactory->CreateSwapChainForHwnd(m_pCommandQueue, (HWND)pWindow->GetHandle(), &m_SwapChainDesc, &m_SwapChainFSD, nullptr,
                                                           (IDXGISwapChain1 **)&m_pSwapChain)))
        {
            LOG_ERROR("Failed to create D3D12 swap chain!");
            return false;
        }

        return true;
    }

    bool D3D12Renderer::CreateBackBuffer()
    {
        // HRESULT hr;
        // ID3D12Texture2D *pBackBuffer = 0;

        // hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

        // if (hr < 0)
        // {
        //     LOG_ERROR("Failed to create D3D11 BackBuffer!");
        //     return false;
        // }

        // auto view = m_TargetManager.Create("renderer://backbuffer", pBackBuffer);
        // m_pDeviceContext->OMSetRenderTargets(1, &view, m_pDepthStencilView);

        // SAFE_RELEASE(pBackBuffer);

        return true;
    }

}  // namespace Lorr

#endif