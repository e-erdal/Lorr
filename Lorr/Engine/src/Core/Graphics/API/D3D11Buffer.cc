#include "D3D11API.hh"

namespace Lorr
{
    ID3D11Buffer *D3D11API::CreateBuffer( const void *pData, size_t sDataSize, D3D11_BIND_FLAG eBindFlags, D3D11_USAGE eUsage )
    {
        ID3D11Buffer *pBuffer = 0;

        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory( &bufferDesc, sizeof( D3D11_BUFFER_DESC ) );

        bufferDesc.ByteWidth = sDataSize;
        bufferDesc.BindFlags = eBindFlags;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        bufferDesc.Usage = eUsage;

        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory( &resourceData, sizeof( D3D11_SUBRESOURCE_DATA ) );
        
        resourceData.pSysMem = pData;

        HRESULT hr = m_pDevice->CreateBuffer( &bufferDesc, &resourceData, &pBuffer );

        if ( FAILED( hr ) )
        {
            PrintError( "Failed to create buffer!" );
            return 0;
        }

        Console::Log("Successfully created buffer.");

        return pBuffer;
    }

}  // namespace Lorr
