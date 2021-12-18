#if LR_BACKEND_D3D12
#include "D3D12Renderer.hh"

// #include "Engine/Graphics/D3D12/D3D12Texture.hh"

static lr::D3D12Renderer *g_D3D12Renderer;

constexpr D3D_FEATURE_LEVEL kMinimumFeatureLevel = { D3D_FEATURE_LEVEL_11_0 };

namespace lr
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

        m_pSwapChain->Init(m_pDevice, m_pFactory, pWindow);
        m_pFactory->MakeWindowAssociation((HWND)pWindow->GetHandle(), DXGI_MWA_NO_ALT_ENTER);

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

        return true;
    }

    bool D3D12Renderer::SelectAdapter(bool highPerformance)
    {
        HRESULT hr;

        DXGI_GPU_PREFERENCE preference = (highPerformance) ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_MINIMUM_POWER;

        IDXGIFactory6 *pFactory6 = nullptr;
        if (SUCCEEDED(m_pFactory->QueryInterface(IID_PPV_ARGS(&pFactory6))))
        {
            for (u32 adapterIdx = 0; SUCCEEDED(pFactory6->EnumAdapterByGpuPreference(adapterIdx, preference, IID_PPV_ARGS(&m_pAdapter))); adapterIdx++)
            {
                DXGI_ADAPTER_DESC1 adapterDesc = {};
                m_pAdapter->GetDesc1(&adapterDesc);

                if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;  // We don't want software adapter

                /// Check if the adapter supports D3D12
                if (SUCCEEDED(D3D12CreateDevice(m_pAdapter, kMinimumFeatureLevel, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }

                SAFE_RELEASE(m_pAdapter);
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
                if (SUCCEEDED(D3D12CreateDevice(m_pAdapter, kMinimumFeatureLevel, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }

                SAFE_RELEASE(m_pAdapter);
            }
        }

        if (!m_pAdapter || FAILED(hr))
        {
            LOG_ERROR("Failed to create D3D12 renderer context, this device does not support D3D12! Select D3D11.");
            return false;
        }

        return true;
    }

    D3D_FEATURE_LEVEL D3D12Renderer::SelectMaxFeatureLevel()
    {
        HRESULT hr;

        constexpr D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
        };

        D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelData = {};
        featureLevelData.NumFeatureLevels = BX_COUNTOF(featureLevels);
        featureLevelData.pFeatureLevelsRequested = featureLevels;

        ID3D12Device *pDevice = nullptr;
        if (FAILED(hr = D3D12CreateDevice(m_pAdapter, kMinimumFeatureLevel, IID_PPV_ARGS(&pDevice))))
        {
            LOG_ERROR("Failed to create D3D12 device.");
            return kMinimumFeatureLevel;
        }

        /// Determine maximum feature level
        pDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelData, sizeof(featureLevelData));
        SAFE_RELEASE(pDevice);

        return featureLevelData.MaxSupportedFeatureLevel;
    }

    bool D3D12Renderer::CreateDevice()
    {
        ZoneScoped;

        LOG_TRACE("Initializing D3D12 device...");

        HRESULT hr;
        u32 flags = 0;

#ifdef _DEBUG
        /// Create debug layer
        flags |= DXGI_CREATE_FACTORY_DEBUG;

        ID3D12Debug1 *pDebug = nullptr;
        D3D12GetDebugInterface(IID_PPV_ARGS(&pDebug));

        pDebug->EnableDebugLayer();
        pDebug->SetEnableGPUBasedValidation(true);

        SAFE_RELEASE(pDebug);
#endif

        /// Find the adapter we wanted
        if (FAILED(hr = CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_pFactory))))
        {
            LOG_ERROR("Failed to create D3D12 factory.");
            return false;
        }

        if (!SelectAdapter(true)) return false;

        m_MaxFeatureLevel = SelectMaxFeatureLevel();

        /// Create actual device
        if (FAILED(hr = D3D12CreateDevice(m_pAdapter, m_MaxFeatureLevel, IID_PPV_ARGS(&m_pDevice))))
        {
            LOG_ERROR("Failed to create D3D12 device.");
            return false;
        }

#ifdef _DEBUG
        ID3D12InfoQueue *pInfoQueue = nullptr;
        m_pDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue));

        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

        SAFE_RELEASE(pInfoQueue);
#endif

        return true;
    }

    void D3D12Renderer::ChangeResolution(u32 width, u32 height)
    {
    }

    void D3D12Renderer::SetViewport(u32 width, u32 height, float fFar, float fNear)
    {
        ZoneScoped;

        // if (!m_IsContextReady) return;

        // D3D12_VIEWPORT vp = {};

        // vp.Width = (float)width;
        // vp.Height = (float)height;
        // vp.MaxDepth = fFar;
        // vp.MinDepth = fNear;
        // vp.TopLeftX = 0;
        // vp.TopLeftY = 0;

        // m_pCommandList->RSSetViewports(1, &vp);
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

}  // namespace lr

#endif