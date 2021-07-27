#include "D3D11API.hh"

namespace Lorr
{
    ID3D11Buffer *D3D11API::CreateBuffer( const void *pData, size_t sDataSize, D3D11_BIND_FLAG eBindFlags, D3D11_USAGE eUsage, uint32_t eAccessFlag,
                                          bool bHasBufferAttach )
    {
        HRESULT hr;
        ID3D11Buffer *pBuffer = 0;

        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory( &bufferDesc, sizeof( D3D11_BUFFER_DESC ) );

        bufferDesc.ByteWidth = sDataSize;
        bufferDesc.BindFlags = eBindFlags;
        bufferDesc.CPUAccessFlags = eAccessFlag;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        bufferDesc.Usage = eUsage;

        if ( bHasBufferAttach )
        {
            D3D11_SUBRESOURCE_DATA resourceData;
            ZeroMemory( &resourceData, sizeof( D3D11_SUBRESOURCE_DATA ) );

            resourceData.pSysMem = pData;

            hr = m_pDevice->CreateBuffer( &bufferDesc, &resourceData, &pBuffer );
        }
        else
        {
            hr = m_pDevice->CreateBuffer( &bufferDesc, 0, &pBuffer );
        }

        if ( FAILED( hr ) )
        {
            PrintError( "Failed to create buffer!" );
            return 0;
        }

        Console::Log( "Successfully created buffer." );

        return pBuffer;
    }

}  // namespace Lorr
