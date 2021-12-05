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

        if (!CreateCommandQueue()) return false;
        if (!CreateSwapChain(pWindow, width, height)) return false;
        m_TargetManager.Init();
        if (!CreateCommandAllocator()) return false;  // This function also creates command list

        SetViewport(width, height, 1.f, 0.0f);

        m_IsContextReady = true;

        LOG_TRACE("Successfully initialized D3D12 device.");

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

    bool D3D12Renderer::CreateDevice()
    {
        ZoneScoped;

        LOG_TRACE("Initializing D3D12 device...");

        HRESULT hr;
        u32 flags = 0;

        /// Create debug layer
#ifdef _DEBUG
        flags |= DXGI_CREATE_FACTORY_DEBUG;

        D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebug));

        m_pDebug->EnableDebugLayer();
        m_pDebug->SetEnableGPUBasedValidation(true);
        m_pDebug->QueryInterface(&m_pDebugDevice);

        // SAFE_RELEASE(pDebug);
#endif

        /// Find the adapter we wanted
        if (FAILED(hr = CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_pFactory))))
        {
            LOG_ERROR("Failed to create D3D12 factory.");
            return false;
        }

        IDXGIFactory6 *pFactory6 = nullptr;
        if (SUCCEEDED(m_pFactory->QueryInterface(IID_PPV_ARGS(&pFactory6))))
        {
            for (u32 adapter = 0; SUCCEEDED(pFactory6->EnumAdapterByGpuPreference(adapter, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_pAdapter)));
                 adapter++)
            {
                DXGI_ADAPTER_DESC1 adapterDesc = {};
                m_pAdapter->GetDesc1(&adapterDesc);

                if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;  // We don't want software adapter

                /// Check if the adapter supports D3D12
                if (SUCCEEDED(D3D12CreateDevice(m_pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        /// We couldn't find any adapter by selected preference, roll everything again
        if (m_pAdapter == nullptr)
        {
            for (u32 adapter = 0; m_pFactory->EnumAdapters1(adapter, &m_pAdapter) != DXGI_ERROR_NOT_FOUND; adapter++)
            {
                DXGI_ADAPTER_DESC1 adapterDesc = {};
                m_pAdapter->GetDesc1(&adapterDesc);

                if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;  // We don't want software adapter

                /// Check if the adapter supports D3D12
                if (SUCCEEDED(D3D12CreateDevice(m_pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        if (!m_pAdapter || FAILED(hr))
        {
            LOG_ERROR("Failed to create D3D12 renderer context, this device does not support D3D12! Select D3D11.");
            return false;
        }

        /// Create actual device
        if (FAILED(hr = D3D12CreateDevice(m_pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice))))
        {
            LOG_ERROR("Failed to create D3D12 device.");
            return false;
        }

        return true;
    }

    bool D3D12Renderer::CreateCommandQueue()
    {
        HRESULT hr;

        /// Create command queue
        D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
        commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        if (FAILED(hr = m_pDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_pCommandQueue))))
        {
            LOG_ERROR("Failed to create D3D12 command queue!");
            return false;
        }

        // if (FAILED(hr = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence))))
        // {
        //     LOG_ERROR("Failed to create D3D12 fence!");
        //     return false;
        // }

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

        m_SwapChainDesc.BufferCount = 2; // Buf 1: back buffer (render), buf 2: front buffer (window)
        m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        m_SwapChainFSD = {};
        m_SwapChainFSD.RefreshRate.Numerator = pWindow->GetDisplay(pWindow->GetUsingMonitor())->RefreshRate;
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

        // TODO: Fullscreen
        m_pFactory->MakeWindowAssociation((HWND)pWindow->GetHandle(), DXGI_MWA_NO_ALT_ENTER);

        m_BackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

        return true;
    }

    bool D3D12Renderer::CreateCommandAllocator()
    {
        HRESULT hr;

        if (FAILED(hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator))))
        {
            LOG_ERROR("Failed to create D3D12 command allocator!");
            return false;
        }

        return true;
    }

    void D3D12Renderer::ChangeResolution(u32 width, u32 height)
    {
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

    void D3D12Renderer::SetClearColor()
    {
    }

    void D3D12Renderer::SetScissor(const glm::vec4 &lrtb)
    {
    }

    void D3D12Renderer::SetDepthFunc(D3D::DepthFunc func, bool depthEnabled)
    {
    }

    void D3D12Renderer::SetCulling(D3D::Cull cull, bool counterClockwise)
    {
    }

    void D3D12Renderer::SetBlend(bool enableBlending, bool alphaCoverage)
    {
    }

    void D3D12Renderer::CreateTarget(const Identifier &ident, u32 width, u32 height, TextureHandle texture, u32 mipLevels)
    {
    }

    void D3D12Renderer::SetCurrentTarget(const Identifier &ident)
    {
    }

    TextureHandle D3D12Renderer::GetTargetTexture(const Identifier &ident)
    {
        return nullptr;
    }

    void D3D12Renderer::UseVertexBuffer(RenderBufferHandle buffer, InputLayout *pLayout, u32 offset)
    {
    }

    void D3D12Renderer::UseIndexBuffer(RenderBufferHandle buffer, bool index32, u32 offset)
    {
    }

    void D3D12Renderer::UseConstantBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot)
    {
    }

    void D3D12Renderer::UseShaderBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot)
    {
    }

    void D3D12Renderer::UseShaderBuffer(TextureHandle texture, RenderBufferTarget target, u32 slot)
    {
    }

    void D3D12Renderer::UseUAV(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot)
    {
    }

    void D3D12Renderer::UseUAV(TextureHandle texture, RenderBufferTarget target, u32 slot)
    {
    }

    void D3D12Renderer::UseSampler(TextureHandle texture, RenderBufferTarget target, u32 slot)
    {
    }

    void D3D12Renderer::UseShader(ShaderHandle shader)
    {
    }

    void D3D12Renderer::TransferResourceData(RenderBufferHandle inputBuffer, RenderBufferHandle outputBuffer)
    {
    }

    void D3D12Renderer::TransferResourceData(TextureHandle inputTexture, TextureHandle outputTexture)
    {
    }

    void D3D12Renderer::Frame(u32 interval)
    {
    }

    void D3D12Renderer::HandlePreFrame()
    {
    }

    void D3D12Renderer::DrawIndexed(u32 indexCount, u32 startIndex, u32 baseVertex)
    {
    }

    void D3D12Renderer::Dispatch(u32 thrX, u32 thrY, u32 thrZ)
    {
    }

    D3D12Renderer *&D3D12Renderer::Get()
    {
        return g_D3D12Renderer;
    }

}  // namespace Lorr

#endif