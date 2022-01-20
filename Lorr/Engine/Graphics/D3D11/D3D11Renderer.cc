#if LR_BACKEND_D3D11

#include "D3D11RenderBuffer.hh"
#include "D3D11Renderer.hh"
#include "D3D11Shader.hh"
#include "D3D11Texture.hh"

static lr::D3D11Renderer *g_D3D11Renderer;

static ID3D11ShaderResourceView *kNullSRV[1] = { nullptr };
constexpr ID3D11UnorderedAccessView *kNullUAV[1] = { nullptr };
constexpr ID3D11Buffer *kNullBuffer[1] = { nullptr };
constexpr ID3D11SamplerState *kNullSampler[1] = { nullptr };

namespace lr
{
    bool D3D11Renderer::CreateDevice()
    {
        ZoneScoped;

        LOG_TRACE("Initializing D3D11 device...");

        HRESULT hr;
        u32 flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#ifdef _DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        constexpr D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

        // feature level that device will select
        D3D_FEATURE_LEVEL currentFeatureLevel;
        if (FAILED(hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION,
                                          &m_pDevice, &currentFeatureLevel, &m_pContext)))

        {
            LOG_ERROR("Failed to create D3D11 device!");
            return false;
        }

        m_TracyContext = TracyD3D11Context(m_pDevice, m_pContext);

        return true;
    }

    bool D3D11Renderer::CreateSwapChain(PlatformWindow *pWindow, u32 width, u32 height)
    {
        ZoneScoped;

        return true;
    }

    bool D3D11Renderer::CreateBackBuffer()
    {
        ZoneScoped;

        ID3D11Texture2D *pBackBuffer = m_pSwapChain->GetBackBuffer();

        auto view = m_TargetManager.Create("renderer://backbuffer", pBackBuffer, true);
        m_pContext->OMSetRenderTargets(1, &view, m_pDepthTexture->GetDepthStencil());

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
        depthData.Format = TextureFormat::R32T, depthData.Width = width, depthData.Height = height;

        m_pDepthTexture = (D3D11Texture *)Texture::Create("renderer://depth-texture", &desc, &depthData);

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

        m_RasterizerDesc.CullMode = (D3D11_CULL_MODE)Cull::Back;
        m_RasterizerDesc.FillMode = D3D11_FILL_SOLID;

        m_RasterizerDesc.DepthClipEnable = true;
        m_RasterizerDesc.DepthBias = 0;
        m_RasterizerDesc.DepthBiasClamp = 0.0f;
        m_RasterizerDesc.SlopeScaledDepthBias = 0.0f;

        m_RasterizerDesc.AntialiasedLineEnable = false;
        m_RasterizerDesc.FrontCounterClockwise = false;
        m_RasterizerDesc.MultisampleEnable = false;
        m_RasterizerDesc.ScissorEnable = true;

        return m_pRasterizerState = m_StateManager.Get(m_RasterizerDesc);
    }

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

        m_pSwapChain = new D3D11SwapChain;

        m_pSwapChain->Init(m_pDevice, pWindow, SwapChainFlags::None);
        m_StateManager.SetDevice(m_pDevice);
        m_TargetManager.Init(m_pDevice);

        if (!CreateDepthTexture(width, height)) return false;
        if (!CreateBackBuffer()) return false;
        if (!CreateDepthStencil()) return false;
        if (!CreateBlendState()) return false;
        if (!CreateRasterizer()) return false;

        m_IsContextReady = true;

        SetViewport(width, height, 1.f, 0.0f);

        LOG_TRACE("Successfully initialized D3D11 device.");

        TextureDesc desc;

        constexpr u32 whiteColor = 0xffffffff;
        TextureData data;
        data.Width = 1;
        data.Height = 1;
        data.DataSize = sizeof(u32);
        data.Data = (u8 *)malloc(data.DataSize);
        data.Format = TextureFormat::RGBA8;
        memcpy(data.Data, &whiteColor, data.DataSize);

        m_PlaceholderTexture = Texture::Create("batcher://placeholder", &desc, &data);

        pWindow->OnResolutionChanged.connect<&D3D11Renderer::Resize>(this);

        return true;
    }

    void D3D11Renderer::Resize(u32 width, u32 height)
    {
        ZoneScoped;

        if (!m_IsContextReady) return;

        m_pContext->OMSetRenderTargets(0, 0, 0);

        m_TargetManager.Release("renderer://backbuffer");
        m_pDepthTexture->Delete();

        m_pSwapChain->Resize(width, height);

        CreateBackBuffer();
        CreateDepthTexture(width, height);
        m_TargetManager.Resize("renderer://postprocess", width, height);
        m_TargetManager.ClearAll(m_pContext);

        SetViewport(width, height, 1.f, 0.f);
    }

    void D3D11Renderer::SetViewport(u32 width, u32 height, float farZ, float nearZ)
    {
        ZoneScoped;

        if (!m_IsContextReady) return;

        D3D11_VIEWPORT vp;
        ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));

        vp.Width = (float)width;
        vp.Height = (float)height;
        vp.MaxDepth = farZ;
        vp.MinDepth = nearZ;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;

        m_pContext->RSSetViewports(1, &vp);
    }

    void D3D11Renderer::SetScissor(const glm::vec4 &lrtb)
    {
        ZoneScoped;

        const D3D11_RECT rect = { (LONG)lrtb.x, (LONG)lrtb.y, (LONG)lrtb.z, (LONG)lrtb.w };

        m_pContext->RSSetScissorRects(1, &rect);
    }

    void D3D11Renderer::SetDepthFunc(DepthFunc func, bool depthEnabled)
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

    void D3D11Renderer::SetCulling(Cull cull, bool counterClockwise)
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

    void D3D11Renderer::SetWireframeState(bool enabled)
    {
        ZoneScoped;

        m_RasterizerDesc.FillMode = enabled ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;

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

    void D3D11Renderer::SetPrimitiveType(PrimitiveType type)
    {
        m_pContext->IASetPrimitiveTopology(D3D::ToDXPrimitive(type));
    }

    void D3D11Renderer::CreateRenderTarget(const Identifier &ident, u32 width, u32 height, TextureHandle texture, u32 mipLevels)
    {
        ZoneScoped;

        auto target = m_TargetManager.Create(ident, width, height, texture, mipLevels);
    }

    void D3D11Renderer::SetRenderTarget(const Identifier &ident)
    {
        ZoneScoped;

        // m_pContext->PSSetShaderResources(0, 1, kNullSRV);

        auto target = m_TargetManager.GetView(ident);
        m_pContext->OMSetRenderTargets(1, &target, m_pDepthTexture->GetDepthStencil());
    }

    void D3D11Renderer::SetRenderTargetClearColor(const Identifier &ident, const glm::vec4 &color)
    {
        ZoneScoped;

        auto target = m_TargetManager.GetView(ident);
        m_pContext->ClearRenderTargetView(target, &color[0]);
        m_pContext->ClearDepthStencilView(m_pDepthTexture->GetDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    TextureHandle D3D11Renderer::GetRenderTargetTexture(const Identifier &ident)
    {
        ZoneScoped;

        return m_TargetManager.GetTexture(ident);
    }

    void D3D11Renderer::SetVertexBuffer(RenderBufferHandle buffer, InputLayout *pLayout, u32 offset)
    {
        ZoneScoped;

        auto stride = pLayout->GetStride();
        ID3D11Buffer *d11Buffer = (ID3D11Buffer *)buffer->GetHandle();
        m_pContext->IASetVertexBuffers(0, 1, &d11Buffer, &stride, &offset);
    }

    void D3D11Renderer::SetIndexBuffer(RenderBufferHandle buffer, bool index32, u32 offset)
    {
        ZoneScoped;

        m_pContext->IASetIndexBuffer((ID3D11Buffer *)buffer->GetHandle(), index32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, offset);
    }

    void D3D11Renderer::SetConstantBuffer(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11Buffer *d11Buffer = *kNullBuffer;
        if (buffer) d11Buffer = (ID3D11Buffer *)buffer->GetHandle();

        if (target & RenderBufferTarget::Vertex) m_pContext->VSSetConstantBuffers(slot, 1, &d11Buffer);
        if (target & RenderBufferTarget::Pixel) m_pContext->PSSetConstantBuffers(slot, 1, &d11Buffer);
        if (target & RenderBufferTarget::Compute) m_pContext->CSSetConstantBuffers(slot, 1, &d11Buffer);
    }

    void D3D11Renderer::SetShaderResource(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11ShaderResourceView **ppSRV = kNullSRV;
        if (buffer) ppSRV[0] = ((D3D11RenderBuffer *)buffer)->GetShaderResource();

        if (target & RenderBufferTarget::Vertex) m_pContext->VSSetShaderResources(slot, 1, ppSRV);
        if (target & RenderBufferTarget::Pixel) m_pContext->PSSetShaderResources(slot, 1, ppSRV);
        if (target & RenderBufferTarget::Compute) m_pContext->CSSetShaderResources(slot, 1, ppSRV);
    }

    void D3D11Renderer::SetUAVResource(RenderBufferHandle buffer, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11UnorderedAccessView *pUAV = *kNullUAV;
        if (buffer) pUAV = ((D3D11RenderBuffer *)buffer)->GetUAV();

        m_pContext->CSSetUnorderedAccessViews(slot, 1, &pUAV, 0);
    }

    void D3D11Renderer::SetShaderResource(TextureHandle texture, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11ShaderResourceView **ppSRV = kNullSRV;
        if (texture) ppSRV[0] = ((D3D11Texture *)texture)->GetShaderResource();

        if (target & RenderBufferTarget::Vertex) m_pContext->VSSetShaderResources(slot, 1, ppSRV);
        if (target & RenderBufferTarget::Pixel) m_pContext->PSSetShaderResources(slot, 1, ppSRV);
        if (target & RenderBufferTarget::Compute) m_pContext->CSSetShaderResources(slot, 1, ppSRV);
    }

    void D3D11Renderer::SetUAVResource(TextureHandle texture, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11UnorderedAccessView *pUAV = *kNullUAV;
        if (texture) pUAV = ((D3D11Texture *)texture)->GetUAV();

        m_pContext->CSSetUnorderedAccessViews(slot, 1, &pUAV, 0);
    }

    void D3D11Renderer::SetSamplerState(TextureHandle texture, RenderBufferTarget target, u32 slot)
    {
        ZoneScoped;

        ID3D11SamplerState *pSamplerState = *kNullSampler;
        if (texture) pSamplerState = ((D3D11Texture *)texture)->GetSampler();

        if (target & RenderBufferTarget::Vertex) m_pContext->VSSetSamplers(slot, 1, &pSamplerState);
        if (target & RenderBufferTarget::Pixel) m_pContext->PSSetSamplers(slot, 1, &pSamplerState);
        if (target & RenderBufferTarget::Compute) m_pContext->CSSetSamplers(slot, 1, &pSamplerState);
    }

    void D3D11Renderer::MapBuffer(RenderBufferHandle buffer, void *pData, u32 dataSize)
    {
        ZoneScoped;

        D3D11_MAPPED_SUBRESOURCE mappedResc = {};
        if (SUCCEEDED(m_pContext->Map((ID3D11Resource *)buffer->GetHandle(), 0, ((D3D11RenderBuffer *)buffer)->GetMappingType(), 0, &mappedResc)))
        {
            memcpy(mappedResc.pData, pData, dataSize);
        }
        m_pContext->Unmap((ID3D11Resource *)buffer->GetHandle(), 0);
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
        // pD11Output->Map(pD11Input);
    }

    void D3D11Renderer::SetShader(ShaderHandle shader)
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

    void D3D11Renderer::BeginFrame()
    {
        /// Reset states
        SetRenderTarget("renderer://backbuffer");

        SetDepthFunc(DepthFunc::Less, true);
        SetCulling(Cull::Back, false);
        SetBlend(true, true);
        SetPrimitiveType(PrimitiveType::TriangleList);

        SetRenderTargetClearColor("renderer://backbuffer", glm::vec4(0.1, 0.1, 0.1, 1.0));
    }

    void D3D11Renderer::Frame()
    {
        ZoneScoped;
        TracyD3D11Zone(m_TracyContext, "Present");

        if (!m_IsContextReady) return;

        if (m_NeedToPresent)
        {
            m_pSwapChain->Present();
            m_pContext->Flush();

            TracyD3D11Collect(m_TracyContext);
        }
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

    D3D11Renderer *&D3D11Renderer::Get()
    {
        return g_D3D11Renderer;
    }

}  // namespace lr

#endif