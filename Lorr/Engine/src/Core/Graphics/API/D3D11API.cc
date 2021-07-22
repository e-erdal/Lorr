#include "D3D11API.hh"

#if _WIN32
    #pragma comment( lib, "d3d11.lib" )
#endif

namespace Lorr
{
    bool D3D11API::Init( Window *pWindow, int iWidth, int iHeight )
    {
        ZoneScoped;

        pWindow->OnResolutionChanged.connect<&D3D11API::ChangeResolution>( this );

        return CreateDevice( pWindow, iWidth, iHeight );
    }

    void D3D11API::SetClearColor( const glm::vec4 &v4Color )
    {
        ZoneScoped;

        m_pDeviceContext->ClearRenderTargetView( m_pRenderTargetView, &v4Color[0] );
        m_pDeviceContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
    }

    void D3D11API::Frame( uint32_t uInterval )
    {
        ZoneScoped;

        if ( m_bNeedToPresent )
        {
            m_pSwapChain->Present( uInterval, 0 );
            // m_pDeviceContext->Flush(); // no idea what to do with this
        }
    }

    void D3D11API::SetViewport( int iWidth, int iHeight, float fFar, float fNear )
    {
        ZoneScoped;

        D3D11_VIEWPORT vp;
        vp.Width = (float) iWidth;
        vp.Height = (float) iHeight;
        vp.MinDepth = fNear;
        vp.MaxDepth = fFar;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;

        m_pDeviceContext->RSSetViewports( 1, &vp );
    }
}  // namespace Lorr