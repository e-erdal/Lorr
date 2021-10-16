#include "D3D11Renderer.hh"

#include "Engine/Graphics/D3D11/D3D11Texture.hh"

static Lorr::D3D11Renderer *g_D3D11Renderer;

namespace Lorr
{
    bool D3D11Renderer::Init(PlatformWindow *pWindow, uint32_t width, uint32_t height)
    {
        ZoneScoped;

        g_D3D11Renderer = this;
        m_CurrentAPI = APIType::D3D11;

        if (!CreateDevice()) return false;

        if (!pWindow)
        {
            LOG_WARN("Console application mode, only initializing D3D11 device.");
            return true;
        }

        if (!CreateSwapChain(pWindow, width, height)) return false;
        if (!CreateBackBuffer()) return false;
        if (!CreateDepthStencil(width, height)) return false;
        if (!CreateRasterizer()) return false;

        m_IsContextReady = true;

        SetViewport(width, height, 1.f, 0.0f);

        LOG_INFO("Successfully initialized D3D11 device.");

        return true;
    }

    void D3D11Renderer::SetClearColor(const glm::vec4 &v4Color)
    {
        ZoneScoped;
        if (!m_IsContextReady) return;

        m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, &v4Color[0]);
        m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void D3D11Renderer::Frame(uint32_t uInterval)
    {
        ZoneScoped;
        if (!m_IsContextReady) return;

        if (m_NeedToPresent)
        {
            m_pSwapChain->Present(uInterval, 0);
            // m_pDeviceContext->Flush(); // no idea what to do with this
        }
    }

    void D3D11Renderer::SetViewport(uint32_t width, uint32_t height, float fFar, float fNear)
    {
        ZoneScoped;

        if (!m_IsContextReady) return;

        D3D11_VIEWPORT vp;
        ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));

        vp.Width = (float)width;
        vp.Height = (float)height;
        vp.MaxDepth = fFar;
        vp.MinDepth = fNear;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;

        m_pDeviceContext->RSSetViewports(1, &vp);
    }

    bool D3D11Renderer::CreateDevice()
    {
        ZoneScoped;

        LOG_INFO("Initializing D3D11 device...");

        HRESULT hr;
        uint32_t flags = 0;

#ifdef _DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        static const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

        // feature level that device will select
        D3D_FEATURE_LEVEL currentFeatureLevel;
        if (FAILED(hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE,  // FIXME: HARDCODED
                                          0, flags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &m_pDevice, &currentFeatureLevel,
                                          &m_pDeviceContext)))

        {
            LOG_ERROR("Failed to create D3D11 device!");
            return false;
        }

        // m_pDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 2, &p );

        return true;
    }

    bool D3D11Renderer::CreateSwapChain(PlatformWindow *pWindow, uint32_t width, uint32_t height)
    {
        HRESULT hr;
        m_SwapChainDesc = {};

        pWindow->OnResolutionChanged.connect<&D3D11Renderer::ChangeResolution>(this);

        // Buffer description
        m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // FIXME: HARDCODED
        m_SwapChainDesc.BufferDesc.Width = width;
        m_SwapChainDesc.BufferDesc.Height = height;
        m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;   // FIXME: HARDCODED
        m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;  // FIXME: HARDCODED
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
        m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
        pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);  // get interface factory from our device
        pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pDXGIFactory);

        hr = pDXGIFactory->CreateSwapChain(m_pDevice, &m_SwapChainDesc, &m_pSwapChain);

        if (hr < 0)
        {
            LOG_ERROR("Failed to create D3D11 swapchain!");
            return false;
        }

        SAFE_RELEASE(pDXGIDevice);
        SAFE_RELEASE(pDXGIAdapter);
        SAFE_RELEASE(pDXGIFactory);

        // Check if we are in fullscreen
        if (pWindow->IsFullscreen())
        {
            LOG_INFO("Going into fullscreen. (swapchain)");
            m_pSwapChain->SetFullscreenState(true, 0);
        }

        return true;
    }

    bool D3D11Renderer::CreateBackBuffer()
    {
        HRESULT hr;
        ID3D11Texture2D *pBackBuffer = 0;

        hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pBackBuffer);

        if (hr < 0)
        {
            LOG_ERROR("Failed to create D3D11 BackBuffer!");
            return false;
        }

        hr = m_pDevice->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView);

        if (hr < 0)
        {
            LOG_ERROR("Failed to create D3D11 RTV!");
            return false;
        }

        SAFE_RELEASE(pBackBuffer);

        return true;
    }

    bool D3D11Renderer::CreateDepthStencil(uint32_t width, uint32_t height)
    {
        HRESULT hr;

        TextureData depthData = {};
        depthData.Format = TEXTURE_FORMAT_DEPTH_32, depthData.Width = width, depthData.Height = height;

        m_DepthTexture = Texture::Create("d3d11-renderer://depth", 0, &depthData);

        m_DepthStencilViewDesc = {};
        m_DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        m_DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

        if (FAILED(hr = m_pDevice->CreateDepthStencilView((ID3D11Texture2D *)m_DepthTexture->GetHandle(), &m_DepthStencilViewDesc, &m_pDepthStencilView)))
        {
            LOG_ERROR("Failed to create D3D11 Depth stencil view!");
            return false;
        }

        m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

        m_DepthStencilDesc = {};

        m_DepthStencilDesc.DepthEnable = true;
        m_DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        m_DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        m_DepthStencilDesc.StencilEnable = false;
        m_DepthStencilDesc.StencilReadMask = 0xFF;
        m_DepthStencilDesc.StencilWriteMask = 0xFF;

        m_DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        m_DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        m_DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        m_DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        m_DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        m_DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        m_DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        m_DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        if (FAILED(hr = m_pDevice->CreateDepthStencilState(&m_DepthStencilDesc, &m_pDepthStencilState)))
        {
            LOG_ERROR("Failed to create D3D11 Depth stencil state!");
            return false;
        }

        m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

        return true;
    }

    bool D3D11Renderer::CreateRasterizer()
    {
        HRESULT hr;
        m_RasterizerDesc = {};

        m_RasterizerDesc.CullMode = D3D11_CULL_NONE;
        m_RasterizerDesc.FillMode = D3D11_FILL_SOLID;

        m_RasterizerDesc.DepthClipEnable = true;
        m_RasterizerDesc.DepthBias = 0;
        m_RasterizerDesc.DepthBiasClamp = 0.0f;
        m_RasterizerDesc.SlopeScaledDepthBias = 0.0f;

        m_RasterizerDesc.AntialiasedLineEnable = false;
        m_RasterizerDesc.FrontCounterClockwise = false;
        m_RasterizerDesc.MultisampleEnable = false;
        m_RasterizerDesc.ScissorEnable = false;

        hr = m_pDevice->CreateRasterizerState(&m_RasterizerDesc, &m_pRasterizerState);

        if (hr < 0)
        {
            LOG_ERROR("Failed to create D3D11 Rasterizer state!");
            return false;
        }

        return true;
    }

    void D3D11Renderer::ChangeResolution(uint32_t width, uint32_t height)
    {
        ZoneScoped;

        if (!m_IsContextReady) return;

        m_pDeviceContext->OMSetRenderTargets(0, 0, 0);

        SAFE_RELEASE(m_pRenderTargetView);
        SAFE_RELEASE(m_pDepthStencilView);
        m_DepthTexture->Delete();

        if (FAILED(m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)))
        {
            LOG_ERROR("Failed to resize swap chain!");
            return;
        }

        CreateBackBuffer();
        CreateDepthStencil(width, height);

        SetViewport(width, height, 1.f, 0.f);
    }

    D3D11Renderer *&D3D11Renderer::Get()
    {
        return g_D3D11Renderer;
    }

    void D3D11Renderer::HandlePreFrame()
    {
        m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
        m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
        m_pDeviceContext->RSSetState(m_pRasterizerState);

        m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void D3D11Renderer::DrawIndexed(uint32_t indexCount)
    {
        m_pDeviceContext->DrawIndexed(indexCount, 0, 0);
    }

    // void D3D11Renderer::HanlePostFrame()
    // {
    // }
}  // namespace Lorr