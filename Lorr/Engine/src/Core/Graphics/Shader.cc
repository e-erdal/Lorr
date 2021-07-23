#include "Shader.hh"

namespace Lorr
{
    Shader::~Shader()
    {
        SAFE_RELEASE( m_pVertexShader );
        SAFE_RELEASE( m_pPixelShader );
    }

    void Shader::Init( D3D11API *pAPI, const char *szVertexPath, const char *szPixelPath, const VertexLayout &vertexLayout )
    {
        ID3DBlob *pVertexBlob = pAPI->CompileShaderFromFile( szVertexPath, VERTEX_SHADER );
        ID3DBlob *pPixelBlob = pAPI->CompileShaderFromFile( szPixelPath, PIXEL_SHADER );

        m_pVertexShader = pAPI->CreateVertexShader( pVertexBlob );
        m_pPixelShader = pAPI->CreatePixelShader( pPixelBlob );

        m_pInputLayout = pAPI->CreateInputLayout( pVertexBlob, vertexLayout );

        SAFE_RELEASE( pVertexBlob );
        SAFE_RELEASE( pPixelBlob );
    }
}  // namespace Lorr