//
// Created on July 28th 2021 by e-erdal.
//

#pragma once

#include "Core/Graphics/API/D3D11API.hh"

#include <obj_loader.h>

namespace Lorr
{
    // const std::vector<float> *pVertices, const std::vector<uint32_t> *pIndices
    typedef std::vector<float> &OBJVertices;
    typedef std::vector<uint32_t> &OBJIndices;

    struct Vertex
    {
        Vertex(){};
        Vertex( glm::vec3 vPos, glm::vec3 vNorm, glm::vec2 vUV, glm::vec4 vColor ) : Pos( vPos ), Norm( vNorm ), UV( vUV ), Color( vColor ){};

        glm::vec3 Pos;
        glm::vec3 Norm{ 0 };
        glm::vec2 UV{ 0 };
        glm::vec4 Color = { 1, 1, 1, 1 };
    };

    class Mesh
    {
    public:
        ~Mesh();

        void Init( OBJVertices Vertices, OBJIndices Indices, QuickOBJLoader::VertexFormat &Format, QuickOBJLoader::Material &Material );

        void InitAsSphere( float fRadius, uint32_t uTessellation );
        void InitAsRect( float fRadius, uint32_t uTessellation );

        void Render();

    private:
        ID3D11Buffer *m_pVertexBuffer = 0;
        ID3D11Buffer *m_pIndexBuffer = 0;

        uint32_t m_IndexCount = 0;
    };
}  // namespace Lorr