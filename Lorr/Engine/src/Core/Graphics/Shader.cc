#include "Shader.hh"

namespace Lorr
{
    Shader::~Shader()
    {
        SAFE_RELEASE( m_pVertexShader );
        SAFE_RELEASE( m_pPixelShader );
    }

    void Shader::Init( D3D11API *pAPI, const wchar_t *wszVertexPath, const wchar_t *wszPixelPath, const VertexLayout &vertexLayout )
    {
        ID3DBlob *pVertexBlob = pAPI->CompileShaderFromFile( wszVertexPath, VERTEX_SHADER );
        if ( pVertexBlob ) printf( "Successfully compiled vertex shader from path '%ws'\n", wszVertexPath );

        ID3DBlob *pPixelBlob = pAPI->CompileShaderFromFile( wszPixelPath, PIXEL_SHADER );
        if ( pPixelBlob ) printf( "Successfully compiled pixel shader from path '%ws'\n", wszPixelPath );

        m_pVertexShader = pAPI->CreateVertexShader( pVertexBlob );
        if ( m_pVertexShader ) Console::Log( "Successfully created vertex shader." );
        m_pPixelShader = pAPI->CreatePixelShader( pPixelBlob );
        if ( m_pPixelShader ) Console::Log( "Successfully created pixel shader." );

        m_pInputLayout = pAPI->CreateInputLayout( pVertexBlob, vertexLayout );
        if ( m_pInputLayout ) Console::Log( "Successfully initialized vertex layout." );

        SAFE_RELEASE( pVertexBlob );
        SAFE_RELEASE( pPixelBlob );
    }
}  // namespace Lorr