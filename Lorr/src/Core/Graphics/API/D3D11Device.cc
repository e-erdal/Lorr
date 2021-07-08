#include "D3D11API.hh"

bool D3D11API::CreateDevice( Window *pSurface, int iWidth, int iHeight )
{
    Console::Log( "Initializing D3D11 device..." );

    HRESULT hr;
    uint32_t flags = 0;

#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
                                                       D3D_FEATURE_LEVEL_9_3,  D3D_FEATURE_LEVEL_9_2,  D3D_FEATURE_LEVEL_9_1 };

    // feature level that device will select
    D3D_FEATURE_LEVEL currentFeatureLevel;

    /////////////////////////////////////////////////////////////////////////
    //      Create D3D11 device
    /////////////////////////////////////////////////////////////////////////

    hr = D3D11CreateDevice( 0, D3D_DRIVER_TYPE_HARDWARE,  // FIXME: HARDCODED
                            0, flags, featureLevels, _countof( featureLevels ), D3D11_SDK_VERSION, &m_pDevice, &currentFeatureLevel, &m_pDeviceContext );

    if ( hr < 0 )
    {
        PrintError( "Failed to create D3D11 device!" );
        return false;
    }

    // m_pDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 2, &p );

    /////////////////////////////////////////////////////////////////////////
    //      Create D3D11 swapchain
    /////////////////////////////////////////////////////////////////////////

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

    // Buffer description
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // FIXME: HARDCODED
    swapChainDesc.BufferDesc.Width = iWidth;
    swapChainDesc.BufferDesc.Height = iHeight;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;   // FIXME: HARDCODED
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;  // FIXME: HARDCODED
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.OutputWindow = (HWND) pSurface->GetHandle();
    swapChainDesc.Windowed = true;  // FIXME: HARDCODED

    IDXGIDevice *pDXGIDevice = 0;
    IDXGIAdapter *pDXGIAdapter = 0;
    IDXGIFactory *pDXGIFactory = 0;
    m_pDevice->QueryInterface( __uuidof( IDXGIDevice ), (void **) &pDXGIDevice );
    pDXGIDevice->GetParent( __uuidof( IDXGIAdapter ), (void **) &pDXGIAdapter );  // get interface factory from our device
    pDXGIAdapter->GetParent( __uuidof( IDXGIFactory ), (void **) &pDXGIFactory );

    hr = pDXGIFactory->CreateSwapChain( m_pDevice, &swapChainDesc, &m_pSwapChain );

    if ( hr < 0 )
    {
        PrintError( "Failed to create D3D11 swapchain!" );
        return false;
    }

    SAFE_RELEASE( pDXGIDevice );
    SAFE_RELEASE( pDXGIAdapter );
    SAFE_RELEASE( pDXGIFactory );

    // Check if we are in fullscreen
    if ( pSurface->IsFullscreen() )
    {
        Console::Log("Going into fullscreen. (swapchain)");
        m_pSwapChain->SetFullscreenState( true, 0 );
    }

    /////////////////////////////////////////////////////////////////////////
    //      Create D3D11 back buffer
    /////////////////////////////////////////////////////////////////////////

    ID3D11Texture2D *pBackBuffer = 0;
    hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID *) &pBackBuffer );

    if ( hr < 0 )
    {
        PrintError( "Failed to create D3D11 BackBuffer!" );
        return false;
    }

    hr = m_pDevice->CreateRenderTargetView( pBackBuffer, 0, &m_pRenderTargetView );

    if ( hr < 0 )
    {
        PrintError( "Failed to create D3D11 RTV!" );
        return false;
    }

    SAFE_RELEASE( pBackBuffer );

    /////////////////////////////////////////////////////////////////////////
    //      Create D3D11 depth stencil texture
    /////////////////////////////////////////////////////////////////////////

    D3D11_TEXTURE2D_DESC dstextureDesc;
    ZeroMemory( &dstextureDesc, sizeof( dstextureDesc ) );
    dstextureDesc.CPUAccessFlags = 0;
    dstextureDesc.MiscFlags = 0;
    dstextureDesc.ArraySize = 1;
    dstextureDesc.MipLevels = 1;
    dstextureDesc.Width = iWidth;
    dstextureDesc.Height = iHeight;
    dstextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dstextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    dstextureDesc.Usage = D3D11_USAGE_DEFAULT;

    dstextureDesc.SampleDesc.Count = 1;
    dstextureDesc.SampleDesc.Quality = 0;

    hr = m_pDevice->CreateTexture2D( &dstextureDesc, 0, &m_pDepthStencilBuffer );

    if ( hr < 0 )
    {
        PrintError( "Failed to create D3D11 Depth stencil texture!" );
        return false;
    }

    /////////////////////////////////////////////////////////////////////////
    //      Create D3D11 depth stencil state
    /////////////////////////////////////////////////////////////////////////

    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory( &depthStencilStateDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

    depthStencilStateDesc.DepthEnable = true;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilStateDesc.StencilEnable = true;
    depthStencilStateDesc.StencilReadMask = 0xFF;
    depthStencilStateDesc.StencilWriteMask = 0xFF;

    depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    hr = m_pDevice->CreateDepthStencilState( &depthStencilStateDesc, &m_pDepthStencilState );

    if ( hr < 0 )
    {
        PrintError( "Failed to create D3D11 Depth stencil state!" );
        return false;
    }

    // Everything is done, now tell device context to apply those settings
    m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilState, 0 );

    /////////////////////////////////////////////////////////////////////////
    //      Create D3D11 depth stencil view
    /////////////////////////////////////////////////////////////////////////

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    hr = m_pDevice->CreateDepthStencilView( m_pDepthStencilBuffer, 0, &m_pDepthStencilView );

    if ( hr < 0 )
    {
        PrintError( "Failed to create D3D11 Depth stencil view!" );
        return false;
    }

    m_pDeviceContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

    /////////////////////////////////////////////////////////////////////////
    //      Create D3D11 rasterizer state
    /////////////////////////////////////////////////////////////////////////

    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory( &rasterizerDesc, sizeof( D3D11_RASTERIZER_DESC ) );
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = true;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;

    hr = m_pDevice->CreateRasterizerState( &rasterizerDesc, &m_pRasterizerState );

    if ( hr < 0 )
    {
        PrintError( "Failed to create D3D11 Rasterizer state!" );
        return false;
    }

    SetViewport( iWidth, iHeight, 99999.f, -99999.f );

    Console::Log( "Successfully initialized D3D11 device." );

    return true;
}