#include "Mesh.hh"

#include "Engine.hh"

#include <glm/gtc/matrix_transform.hpp>

namespace Lorr
{
    Mesh::~Mesh()
    {
    }

    void Mesh::Init( OBJVertices Vertices, OBJIndices Indices, QuickOBJLoader::VertexFormat &Format, QuickOBJLoader::Material &Material )
    {
        size_t offset = 0;

        std::vector<Vertex> vertexArray;
        std::vector<uint32_t> indexArray;

        while ( offset != Vertices.size() )
        {
            Vertex vertex;

            if ( Format.positionElementOffset.has_value() )
            {
                float x = Vertices[offset + 0];
                float y = Vertices[offset + 1];
                float z = Vertices[offset + 2];
                offset += 3;

                vertex.Pos = { x, y, z };
            }

            if ( Format.normalElementOffset.has_value() )
            {
                float x = Vertices[offset + 0];
                float y = Vertices[offset + 1];
                float z = Vertices[offset + 2];
                offset += 3;

                vertex.Norm = { x, y, z };
            }

            if ( Format.textureCoordinatesElementOffset.has_value() )
            {
                float u = Vertices[offset + 0];
                float v = Vertices[offset + 1];
                offset += 2;

                // vertex.UV = { u, v };
            }

            vertex.Color = { Material.R, Material.G, Material.B, 1.f };
            vertexArray.push_back( vertex );
        }

        indexArray.swap( Indices );

        D3D11API *pAPI = Lorr::GetEngine()->GetAPI();

        m_pVertexBuffer = pAPI->CreateBuffer( &vertexArray[0], vertexArray.size() * sizeof( Vertex ), D3D11_BIND_VERTEX_BUFFER );
        m_pIndexBuffer = pAPI->CreateBuffer( &indexArray[0], indexArray.size() * sizeof( uint32_t ), D3D11_BIND_INDEX_BUFFER );

        m_IndexCount = indexArray.size();
    }

    void Mesh::InitAsSphere( float fRadius, uint32_t uTessellation )
    {
        std::vector<Vertex> vertexArray;
        std::vector<uint32_t> indexArray;

        if ( uTessellation < 3 )
        {
            Console::Fatal( "What? Are you trying to create triangle???" );
            return;
        }

        int segVert = uTessellation;
        int segHor = uTessellation * 2;

        for ( size_t i = 0; i <= segVert; i++ )
        {
            float v = 1 - (float) i / segVert;

            float latitude = ( i * ( Math::PI2 ) / segVert ) - Math::PIDIV2;
            float dy;
            float dxz;

            Math::ScalarSinCos( &dy, &dxz, latitude );

            // Single latitude ring for horizontal segment
            for ( size_t e = 0; e <= segHor; e++ )
            {
                float u = 1 - (float) i / segHor;

                float longitude = e * Math::PI2 / segHor;
                float dx;
                float dz;

                Math::ScalarSinCos( &dx, &dz, longitude );

                dx *= dxz;
                dz *= dxz;

                glm::vec3 normal( dx, dy, dz );
                glm::vec2 uv( u, v );

                vertexArray.push_back( Vertex( normal * fRadius, normal, uv, { 1.f, 1.f, 1.f, 1.f } ) );
            }
        }

        int stride = segHor + 1;

        for ( int x = 0; x < segVert; x++ )
        {
            for ( int y = 0; y < segHor; y++ )
            {
                int nextX = x + 1;
                int nextY = ( y + 1 ) % stride;

                indexArray.resize( indexArray.size() + 6 );

                indexArray.push_back( x * stride + nextY );
                indexArray.push_back( nextX * stride + y );
                indexArray.push_back( x * stride + y );

                indexArray.push_back( nextX * stride + nextY );
                indexArray.push_back( nextX * stride + y );
                indexArray.push_back( x * stride + nextY );
            }
        }

        D3D11API *pAPI = Lorr::GetEngine()->GetAPI();

        m_pVertexBuffer = pAPI->CreateBuffer( &vertexArray[0], vertexArray.size() * sizeof( Vertex ), D3D11_BIND_VERTEX_BUFFER );
        m_pIndexBuffer = pAPI->CreateBuffer( &indexArray[0], indexArray.size() * sizeof( uint32_t ), D3D11_BIND_INDEX_BUFFER );

        m_IndexCount = indexArray.size();
    }

    void Mesh::InitAsRect( float fRadius, uint32_t uTessellation )
    {
        D3D11API *pAPI = Lorr::GetEngine()->GetAPI();

        uint32_t indexOffset = 0;
        std::vector<Vertex> vertexArray;
        std::vector<uint32_t> indexArray;
        float tHalf = (float) uTessellation / 2;

        for ( uint32_t y = 0; y < uTessellation; y++ )
        {
            vertexArray.reserve( uTessellation * 4 * sizeof( Vertex ) );
            indexArray.reserve( uTessellation * 6 * sizeof( uint32_t ) );

            for ( uint32_t x = 0; x < uTessellation; x++ )
            {
                vertexArray.push_back(
                    Vertex( ( glm::vec3{ x + 0 - tHalf, y + 0 - tHalf, 0 } ) * fRadius, { 0, 0, -1 }, { 0, 1 }, { 1.f, 1.f, 1.f, 1.f } ) );  // bottom left
                vertexArray.push_back(
                    Vertex( ( glm::vec3{ x + 0 - tHalf, y + 1 - tHalf, 0 } ) * fRadius, { 0, 0, -1 }, { 1, 0 }, { 1.f, 1.f, 1.f, 1.f } ) );  // top left
                vertexArray.push_back(
                    Vertex( ( glm::vec3{ x + 1 - tHalf, y + 1 - tHalf, 0 } ) * fRadius, { 0, 0, -1 }, { 1, 1 }, { 1.f, 1.f, 1.f, 1.f } ) );  // top right
                vertexArray.push_back(
                    Vertex( ( glm::vec3{ x + 1 - tHalf, y + 0 - tHalf, 0 } ) * fRadius, { 0, 0, -1 }, { 0, 1 }, { 1.f, 1.f, 1.f, 1.f } ) );  // bottom right

                indexArray.push_back( indexOffset + 0 );
                indexArray.push_back( indexOffset + 1 );
                indexArray.push_back( indexOffset + 2 );

                indexArray.push_back( indexOffset + 2 );
                indexArray.push_back( indexOffset + 3 );
                indexArray.push_back( indexOffset + 0 );

                indexOffset += 4;
            }
        }

        m_pVertexBuffer = pAPI->CreateBuffer( &vertexArray[0], vertexArray.size() * sizeof( Vertex ), D3D11_BIND_VERTEX_BUFFER );
        m_pIndexBuffer = pAPI->CreateBuffer( &indexArray[0], indexArray.size() * sizeof( uint32_t ), D3D11_BIND_INDEX_BUFFER );

        m_IndexCount = indexArray.size();
    }

    void Mesh::Render()
    {
        ID3D11DeviceContext *pContext = Lorr::GetEngine()->GetAPI()->GetDeviceContext();

        uint32_t vertexStride = sizeof( Vertex );
        uint32_t offset = 0;

        pContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &vertexStride, &offset );
        pContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
        pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

        pContext->DrawIndexed( m_IndexCount, 0, 0 );
    }
}  // namespace Lorr