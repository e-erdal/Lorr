#if LR_BACKEND_D3D11

#include "D3D11RenderBuffer.hh"
#include "D3D11Renderer.hh"
#include "D3D11Shader.hh"
#include "D3D11Texture.hh"

static Lorr::D3D11Renderer *g_D3D11Renderer;

constexpr ID3D11ShaderResourceView *kNullSRV[1] = { nullptr };
constexpr ID3D11UnorderedAccessView *kNullUAV[1] = { nullptr };
constexpr ID3D11Buffer *kNullBuffer[1] = { nullptr };

namespace Lorr
{
    bool D3D11Renderer::Init(PlatformWindow *pWindow, u32 width, u32 height)
    {
        ZoneScoped;

        g_D3D11Renderer = this;
        m_CurrentAPI = RendererType::D3D11;

        if (!CreateDevice()) return false;

        if (!pWindow)
        {
            LOG_WARN("Console application mode, only initializing D3D11 device.");
            return true;
        }

        // Create default members we needed
        m_StateManager.SetDevice(m_pDevice);
        m_TargetManager.Init(m_pDevice);
        if (!CreateSwapChain(pWindow, width, height)) return false;
        if (!CreateBackBuffer()) return false;
        if (!CreateDepthTexture(width, height)) return false;
        if (!CreateDepthStencil()) return false;
        if (!CreateBlendState()) return false;
        if (!CreateRasterizer()) return false;

        m_IsContextReady = true;

        SetViewport(width, height, 1.f, 0.0f);

        LOG_INFO("Successfully initialized D3D11 device.");

        TextureDesc desc;

        constexpr u32 whiteColor = 0xffffffff;
        TextureData data;
        data.Width = 1;
        data.Height = 1;
        data.DataSize = sizeof(u32);
        data.Data = (u8 *)&whiteColor;

        m_PlaceholderTexture = Texture::Create("batcher://placeholder", &desc, &data);

        return true;
    }

    void D3D11Renderer::SetClearColor()
    {
        ZoneScoped;

        if (!m_IsContextReady) return;

        m_TargetManager.ClearAll(m_pContext);
        m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void D3D11Renderer::SetScissor(const glm::vec4 &lrtb)
    {
        ZoneScoped;

        D3D11_RECT rect;
        rect.left = lrtb.x;
        rect.right = lrtb.y;
        rect.top = lrtb.z;
        rect.bottom = lrtb.w;

        m_pContext->RSSetScissorRects(1, &rect);
    }

    void D3D11Renderer::SetDepthFunc(D3D::DepthFunc func, bool depthEnabled)
    {
        ZoneScoped;

        m_DepthStencilDesc.DepthEnable = depthEnabled;
        m_DepthStencilDesc.StencilEnable = depthEnabled;
        m_DepthStencilDesc.DepthFunc = (D3D11_COMPARISON_FUNC)func;

        auto state = m_StateManager.Get(m_DepthStencilDesc);
        if (m_pDepthStencilState != state)
        {
            m_pDepthStencilState = state;
            m_pContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
        }
    }

    void D3D11Renderer::SetCulling(D3D::Cull cull, bool counterClockwise)
    {
        ZoneScoped;

        m_RasterizerDesc.FrontCounterClockwise = counterClockwise;
        m_RasterizerDesc.CullMode = (D3D11_CULL_MODE)cull;

        auto state = m_StateManager.Get(m_RasterizerDesc);
        if (m_pRasterizerState != state)
        {
            m_pRasterizerState = state;
            m_pContext->RSSetState(m_pRasterizerState);
        }
    }

    void D3D11Renderer::SetBlend(bool enableBlending, bool alphaCoverage)
    {
        ZoneScoped;

        m_BlendDesc.AlphaToCoverageEnable = alphaCoverage;
        m_BlendDesc.RenderTarget[0].BlendEnable = enableBlending;

        auto state = m_StateManager.Get(m_BlendDesc);
        if (m_pBlendState != state)
        {
            m_pBlendState = state;
            glm::vec4 factor = { 1, 1, 1, 1 };
            m_pContext->OMSetBlendState(m_pBlendState, &factor[0], 0xffffffff);
        }
    }

    void D3D11Renderer::CreateTarget(const Identifier &ident, u32 width, u32 height, TextureHandle texture, u32 mipLevels)
    {
        ZoneScoped;

        auto target = m_TargetManager.Create(ident, width, height, texture, mipLevels);
    }

    void D3D11Renderer::SetCurrentTarget(const Identifier &ident)
    {
        ZoneScoped;

        m_pContext->PSSetShaderResources(0, 1, kNullSRV);

        auto target = m_TargetManager.GetView(ident);
        m_pContext->OMSetRenderTargets(1, &target, m_pDepthStencilView);
    }

    TextureHandle D3D11Renderer::GetTargetTexture(const Identifier &ident)
    {
        ZoneScoped;

        return m_TargetManager.GetTexture(ident);
    }

    void D3D11Renderer::UseVertexBuffer(RenderBufferHandle buffer, InputLayout *pLayout, u32 offset)
    {
        ZoneScoped;

        auto stride = pLayout->GetStride();
        ID3D11Buffer *d11Buffer = (ID3D11Buffer *)buffer->GetHandle();
        m_pContext->IASetVertexBuffers(0, 1, &d11Buffer, &stride, &offset);
    }

    void D3D11Renderer::UseIndexBuffer(RenderBufferHandle buffer, bool index32, u32 offset)
    {
        ZoneScoped;

        m_pContext->IASetIndexBuffer((ID3D11Buffer *)buffer->GetHandle(), index32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, offset);
    }

    void D3D11Renderer::UseConstantBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11Buffer *d11Buffer = *kNullBuffer;
        if (buffer) d11Buffer = (ID3D11Buffer *)buffer->GetHandle();

        if (target & RenderBufferTarget::Vertex) m_pContext->VSSetConstantBuffers(slot, 1, &d11Buffer);
        if (target & RenderBufferTarget::Pixel) m_pContext->PSSetConstantBuffers(slot, 1, &d11Buffer);
        if (target & RenderBufferTarget::Compute) m_pContext->CSSetConstantBuffers(slot, 1, &d11Buffer);
    }

    void D3D11Renderer::UseShaderBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11ShaderResourceView *pSRV = *kNullSRV;
        if (buffer) pSRV = ((D3D11RenderBuffer *)buffer)->GetShaderResource();

        if (target & RenderBufferTarget::Vertex) m_pContext->VSSetShaderResources(slot, 1, &pSRV);
        if (target & RenderBufferTarget::Pixel) m_pContext->PSSetShaderResources(slot, 1, &pSRV);
        if (target & RenderBufferTarget::Compute) m_pContext->CSSetShaderResources(slot, 1, &pSRV);
    }

    void D3D11Renderer::UseShaderBuffer(TextureHandle texture, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11ShaderResourceView *pSRV = *kNullSRV;
        if (texture) pSRV = ((D3D11Texture *)texture)->GetShaderResource();

        if (target & RenderBufferTarget::Vertex) m_pContext->VSSetShaderResources(slot, 1, &pSRV);
        if (target & RenderBufferTarget::Pixel) m_pContext->PSSetShaderResources(slot, 1, &pSRV);
        if (target & RenderBufferTarget::Compute) m_pContext->CSSetShaderResources(slot, 1, &pSRV);
    }

    void D3D11Renderer::UseUAV(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11UnorderedAccessView *pUAV = *kNullUAV;
        if (buffer) pUAV = ((D3D11RenderBuffer *)buffer)->GetUAV();

        m_pContext->CSSetUnorderedAccessViews(slot, 1, &pUAV, 0);
    }

    void D3D11Renderer::UseUAV(TextureHandle texture, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11UnorderedAccessView *pUAV = *kNullUAV;
        if (texture) pUAV = ((D3D11Texture *)texture)->GetUAV();

        m_pContext->CSSetUnorderedAccessViews(slot, 1, &pUAV, 0);
    }

    void D3D11Renderer::UseShader(ShaderHandle shader)
    {
        ZoneScoped;

        D3D11Shader *d11Shader = (D3D11Shader *)shader;

        switch (d11Shader->GetType())
        {
            case ShaderType::Vertex:
                m_pContext->IASetInputLayout(d11Shader->GetLayout());
                m_pContext->VSSetShader(d11Shader->GetShader().m_pVertexShader, 0, 0);
                break;
            case ShaderType::Pixel: m_pContext->PSSetShader(d11Shader->GetShader().m_pPixelShader, 0, 0); break;
            case ShaderType::Compute: m_pContext->CSSetShader(d11Shader->GetShader().m_pComputeShader, 0, 0); break;
        }
    }

    void D3D11Renderer::TransferResourceData(RenderBufferHandle inputBuffer, RenderBufferHandle outputBuffer)
    {
        ZoneScoped;

        m_pContext->CopyResource((ID3D11Resource *)outputBuffer->GetHandle(), (ID3D11Resource *)inputBuffer->GetHandle());
    }

    void D3D11Renderer::TransferResourceData(TextureHandle inputTexture, TextureHandle outputTexture)
    {
        ZoneScoped;

        D3D11Texture *pD11Input = (D3D11Texture *)inputTexture;
        D3D11Texture *pD11Output = (D3D11Texture *)outputTexture;
        m_pContext->CopyResource((ID3D11Resource *)pD11Output->GetHandle(), (ID3D11Resource *)pD11Input->GetHandle());
        // pD11Output->Map();
    }

    void D3D11Renderer::Frame(u32 uInterval)
    {
        ZoneScoped;

        if (!m_IsContextReady) return;

        if (m_NeedToPresent)
        {
            m_pSwapChain->Present(uInterval, 0);
            m_pContext->Flush();  // no idea what to do with this
        }
    }

    void D3D11Renderer::SetViewport(u32 width, u32 height, float fFar, float fNear)
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

        m_pContext->RSSetViewports(1, &vp);
    }

    bool D3D11Renderer::CreateDevice()
    {
        ZoneScoped;

        LOG_INFO("Initializing D3D11 device...");

        HRESULT hr;
        u32 flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#ifdef _DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        static const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

        // feature level that device will select
        D3D_FEATURE_LEVEL currentFeatureLevel;
        if (FAILED(hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &m_pDevice,
                                          &currentFeatureLevel, &m_pContext)))

        {
            LOG_ERROR("Failed to create D3D11 device!");
            return false;
        }

        return true;
    }

    bool D3D11Renderer::CreateSwapChain(PlatformWindow *pWindow, u32 width, u32 height)
    {
        ZoneScoped;

        HRESULT hr;
        m_SwapChainDesc = {};

        pWindow->OnResolutionChanged.connect<&D3D11Renderer::ChangeResolution>(this);

        // Buffer description
        m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
        ZoneScoped;

        HRESULT hr;
        ID3D11Texture2D *pBackBuffer = 0;

        hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

        if (hr < 0)
        {
            LOG_ERROR("Failed to create D3D11 BackBuffer!");
            return false;
        }

        auto view = m_TargetManager.Create("renderer://backbuffer", pBackBuffer);
        m_pContext->OMSetRenderTargets(1, &view, m_pDepthStencilView);

        SAFE_RELEASE(pBackBuffer);

        return true;
    }

    bool D3D11Renderer::CreateDepthTexture(u32 width, u32 height)
    {
        ZoneScoped;

        HRESULT hr;

        TextureDesc desc = {};
        desc.Type = TEXTURE_TYPE_DEPTH;

        TextureData depthData = {};
        depthData.Format = TEXTURE_FORMAT_R32_TYPELESS, depthData.Width = width, depthData.Height = height;

        m_DepthTexture = Texture::Create("d3d11-renderer://depth", &desc, &depthData);

        m_DepthStencilViewDesc = {};
        m_DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        m_DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

        if (FAILED(hr = m_pDevice->CreateDepthStencilView((ID3D11Texture2D *)m_DepthTexture->GetHandle(), &m_DepthStencilViewDesc, &m_pDepthStencilView)))
        {
            LOG_ERROR("Failed to create D3D11 Depth stencil view!");
            return false;
        }

        return true;
    }

    bool D3D11Renderer::CreateDepthStencil()
    {
        ZoneScoped;

        m_DepthStencilDesc.DepthEnable = true;
        m_DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        m_DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        m_DepthStencilDesc.StencilEnable = true;
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

        return m_pDepthStencilState = m_StateManager.Get(m_DepthStencilDesc);
    }

    bool D3D11Renderer::CreateBlendState()
    {
        ZoneScoped;

        m_BlendDesc.AlphaToCoverageEnable = false;
        m_BlendDesc.RenderTarget[0].BlendEnable = true;
        m_BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        m_BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        m_BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        m_BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        m_BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        m_BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        m_BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        return m_pBlendState = m_StateManager.Get(m_BlendDesc);
    }

    bool D3D11Renderer::CreateRasterizer()
    {
        ZoneScoped;

        m_RasterizerDesc.CullMode = (D3D11_CULL_MODE)D3D::Cull::Back;
        m_RasterizerDesc.FillMode = D3D11_FILL_SOLID;

        m_RasterizerDesc.DepthClipEnable = true;
        m_RasterizerDesc.DepthBias = 0;
        m_RasterizerDesc.DepthBiasClamp = 0.0f;
        m_RasterizerDesc.SlopeScaledDepthBias = 0.0f;

        m_RasterizerDesc.AntialiasedLineEnable = false;
        m_RasterizerDesc.FrontCounterClockwise = false;
        m_RasterizerDesc.MultisampleEnable = false;
        m_RasterizerDesc.ScissorEnable = false;

        return m_pRasterizerState = m_StateManager.Get(m_RasterizerDesc);
    }

    void D3D11Renderer::ChangeResolution(u32 width, u32 height)
    {
        ZoneScoped;

        if (!m_IsContextReady) return;

        m_pContext->OMSetRenderTargets(0, 0, 0);

        m_TargetManager.Release("renderer://backbuffer");
        SAFE_RELEASE(m_pDepthStencilView);
        m_DepthTexture->Delete();

        if (FAILED(m_pSwapChain->ResizeBuffers(0, 0, 0, m_SwapChainDesc.BufferDesc.Format, 0)))
        {
            LOG_ERROR("Failed to resize swap chain!");
            return;
        }

        CreateBackBuffer();
        CreateDepthTexture(width, height);

        SetViewport(width, height, 1.f, 0.f);
    }

    D3D11Renderer *&D3D11Renderer::Get()
    {
        return g_D3D11Renderer;
    }

    void D3D11Renderer::HandlePreFrame()
    {
        ZoneScoped;

        SetDepthFunc(D3D::DepthFunc::Less, true);
        SetCulling(D3D::Cull::Back, true);
        SetBlend(true, true);

        SetClearColor();
        SetCurrentTarget("renderer://backbuffer");

        m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void D3D11Renderer::DrawIndexed(u32 indexCount, u32 startIndex, u32 baseVertex)
    {
        ZoneScoped;

        m_pContext->DrawIndexed(indexCount, startIndex, baseVertex);
    }

    void D3D11Renderer::Dispatch(u32 thrX, u32 thrY, u32 thrZ)
    {
        ZoneScoped;

        m_pContext->Dispatch(thrX, thrY, thrZ);
    }

}  // namespace Lorr

#endif