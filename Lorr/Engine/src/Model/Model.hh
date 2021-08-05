//
// Created on July 28th 2021 by e-erdal.
//

#pragma once

#include "Mesh.hh"

namespace Lorr
{
    class Model
    {
    public:
        ~Model();

        void Init( const char *szPath );

        void InitAsSphere(float fRadius, uint32_t uTessellation);
        void InitAsRect(float fRadius, uint32_t uTessellation);

        void Render();

    private:
        std::vector<Mesh> m_vMeshes;
    };
}  // namespace Lorr