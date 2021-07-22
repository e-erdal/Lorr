#include "D3D11API.hh"

#include <d3dCompiler.h>

namespace Lorr
{
    ID3DBlob *D3D11API::CompileShaderFromFile( const char *szPath, ShaderType eShaderType, const char *szEntryPoint )
    {
        HRESULT hr;
        ID3DBlob *errorBlob = 0;
        ID3DBlob *shaderBlob = 0;

        uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
        flags |= D3DCOMPILE_DEBUG;
#endif

        hr = D3DCompileFromFile( (LPCWSTR) szPath, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, ShaderTypeToLatestProfile( eShaderType ), flags, 0, &shaderBlob,
                                 &errorBlob );

        if ( FAILED( hr ) && errorBlob )
        {
            if ( errorBlob )
            {
                printf( "Error blob: %s\n", (char *) errorBlob->GetBufferPointer() );
                PrintError( "Failed to compile shader from file!" );
                SAFE_RELEASE( shaderBlob );
                SAFE_RELEASE( errorBlob );
                return 0;
            }
        }

        return shaderBlob;
    }

    ID3D11InputLayout *D3D11API::CreateInputLayout( ID3DBlob *pVertexShaderBlob, const D3D11_INPUT_ELEMENT_DESC &VertexLayout )
    {
        HRESULT hr;

        hr = m_pDevice->CreateInputLayout( VertexLayout, _countof( VertexLayout ), pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(),
                                           &VertexLayout );

        return 0;
    }

    const char *D3D11API::GetLatestProfileVS( D3D_FEATURE_LEVEL eFeatureLevel )
    {
        switch ( eFeatureLevel )
        {
        case D3D_FEATURE_LEVEL_11_1:
        case D3D_FEATURE_LEVEL_11_0: return "vs_5_0";

        case D3D_FEATURE_LEVEL_10_1: return "vs_4_1";
        case D3D_FEATURE_LEVEL_10_0: return "vs_4_0";

        case D3D_FEATURE_LEVEL_9_3: return "vs_4_0_level_9_3";
        case D3D_FEATURE_LEVEL_9_2:
        case D3D_FEATURE_LEVEL_9_1: return "vs_4_0_level_9_1";

        default: return "";
        }
    }

    const char *D3D11API::GetLatestProfilePS( D3D_FEATURE_LEVEL eFeatureLevel )
    {
        switch ( eFeatureLevel )
        {
        case D3D_FEATURE_LEVEL_11_1:
        case D3D_FEATURE_LEVEL_11_0: return "ps_5_0";

        case D3D_FEATURE_LEVEL_10_1: return "ps_4_1";
        case D3D_FEATURE_LEVEL_10_0: return "ps_4_0";

        case D3D_FEATURE_LEVEL_9_3: return "ps_4_0_level_9_3";
        case D3D_FEATURE_LEVEL_9_2:
        case D3D_FEATURE_LEVEL_9_1: return "ps_4_0_level_9_1";

        default: return "";
        }
    }

    const char *D3D11API::ShaderTypeToLatestProfile( ShaderType eShaderType )
    {
        D3D_FEATURE_LEVEL featureLevel = m_pDevice->GetFeatureLevel();

        switch ( eShaderType )
        {
        case VERTEX_SHADER: return GetLatestProfileVS( featureLevel );
        case PIXEL_SHADER: return GetLatestProfilePS( featureLevel );
        case GEOMETRY_SHADER:
        case HULL_SHADER:
        default: return "";
        }
    }
}  // namespace Lorr