//
// Created on Saturday 16th October 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Common/InputLayout.hh"
#include "Engine/Graphics/Common/IRenderBuffer.hh"

#include <obj_loader.h>

namespace Lorr
{
    typedef std::vector<float> OBJVertices;
    typedef std::vector<uint32_t> OBJIndices;
    static InputLayout kMeshLayout = {
        { VertexAttribType::Vec3, "POSITION" },
        { VertexAttribType::Vec3, "NORMAL" },
        { VertexAttribType::Vec4, "COLOR" },
    };

    struct MeshVertex
    {
        glm::vec3 Pos;
        glm::vec3 Norm = {};
        glm::vec4 Color = { 1, 1, 1, 1 };
    };

    class Mesh
    {
    public:
        Mesh() = default;

        void Init(OBJVertices &vertices, uint32_t indices, QuickOBJLoader::VertexFormat &format, QuickOBJLoader::Material &material);

    public:
        const auto &GetVertices() const
        {
            return m_Vertices;
        }

    private:
        std::vector<MeshVertex> m_Vertices;
        uint32_t m_IndexOffset = 0;
    };

}  // namespace Lorr