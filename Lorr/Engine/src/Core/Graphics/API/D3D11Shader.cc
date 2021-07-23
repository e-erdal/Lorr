#include "D3D11API.hh"

#include "Core/Graphics/Shader.hh"

#include <d3dCompiler.h>

namespace Lorr
{
    DXGI_FORMAT VertexAttribToDXGIFormat( VertexAttribType eAttrib )
    {
        switch ( eAttrib )
        {
        case VertexAttribType::Float: return DXGI_FORMAT_R32_FLOAT;
        case VertexAttribType::Vec2: return DXGI_FORMAT_R32G32_FLOAT;
        case VertexAttribType::Vec3: return DXGI_FORMAT_R32G32B32_FLOAT;
        case VertexAttribType::Vec4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        // case VertexAttribType::Mat4: return ?; // NO
        case VertexAttribType::Int: return DXGI_FORMAT_R32_SINT;
        case VertexAttribType::IVec2: return DXGI_FORMAT_R32G32_SINT;
        case VertexAttribType::IVec3: return DXGI_FORMAT_R32G32B32_SINT;
        case VertexAttribType::IVec4: return DXGI_FORMAT_R32G32B32A32_SINT;
        default: return DXGI_FORMAT_UNKNOWN;
        }
    }

    ID3DBlob *D3D11API::CompileShaderFromFile( const char *szPath, ShaderType eShaderType, const char *szEntryPoint )
    {
        HRESULT hr;
        ID3DBlob *pErrorBlob = 0;
        ID3DBlob *pShaderBlob = 0;

        uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
        flags |= D3DCOMPILE_DEBUG;
#endif

        hr = D3DCompileFromFile( (LPCWSTR) szPath, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, ShaderTypeToLatestProfile( eShaderType ), flags, 0, &pShaderBlob,
                                 &pErrorBlob );

        if ( FAILED( hr ) && pErrorBlob )
        {
            printf( "Error blob: %s\n", (char *) pErrorBlob->GetBufferPointer() );
            PrintError( "Failed to compile shader from file!" );
            SAFE_RELEASE( pShaderBlob );
            SAFE_RELEASE( pErrorBlob );
            return 0;
        }

        return pShaderBlob;
    }

    ID3D11InputLayout *D3D11API::CreateInputLayout( ID3DBlob *pVertexShaderBlob, const VertexLayout &vertexLayout )
    {
        HRESULT hr;
        ID3D11InputLayout *pLayout = 0;

        std::vector<D3D11_INPUT_ELEMENT_DESC> d3dInput;  // hehe
        for ( auto &element : vertexLayout.GetElements() )
            d3dInput.push_back( { element.Name.c_str(), 0, VertexAttribToDXGIFormat( element.Type ), 0, element.Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 } );

        hr = m_pDevice->CreateInputLayout( &d3dInput[0], vertexLayout.GetStride(), pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), &pLayout );

        return pLayout;
    }

    ID3D11VertexShader *D3D11API::CreateVertexShader( ID3DBlob *pVertexShaderBlob )
    {
        ID3D11VertexShader *pVertexShader = 0;
        HRESULT hr = m_pDevice->CreateVertexShader( pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), nullptr, &pVertexShader );

        if ( FAILED( hr ) )
        {
            PrintError( "Failed to create vertex shader!" );
            return 0;
        }

        return pVertexShader;
    }

    ID3D11PixelShader *D3D11API::CreatePixelShader( ID3DBlob *pPixelShaderBlob )
    {
        ID3D11PixelShader *pPixelShader = 0;
        HRESULT hr = m_pDevice->CreatePixelShader( pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), nullptr, &pPixelShader );

        if ( FAILED( hr ) )
        {
            PrintError( "Failed to create pixel shader!" );
            return 0;
        }

        return pPixelShader;
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