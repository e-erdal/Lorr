//
// Created on Saturday 16th October 2021 by e-erdal
//

#pragma once

#include "Mesh.hh"

#include "Engine/Graphics/Common/IRenderBuffer.hh"

namespace Lorr
{
    class Model
    {
    public:
        Model() = default;

        void Init(const std::string &path);

        void AddSphere(float radius, uint32_t tessellation, TextureHandle texture);

        void Render();

    private:
        std::vector<Mesh> m_Meshes;

        uint32_t m_IndexCount = 0;
    };

}  // namespace Lorr