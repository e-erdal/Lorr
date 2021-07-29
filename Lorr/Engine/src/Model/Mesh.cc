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
            Vertex vertex{};

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