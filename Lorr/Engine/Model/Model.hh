//
// Created on Saturday 16th October 2021 by e-erdal
//

#pragma once

#include "Mesh.hh"

#include "Engine/Graphics/Common/IRenderBuffer.hh"

namespace Lorr
{
    struct ModelDesc
    {
        bool Dynamic = false;
    };

    struct ModelData
    {
        std::vector<FileMesh> Meshes;
    };

    struct ModelRenderBuffer
    {
        glm::mat4 Matrix;
    };

    class Model : public IResource<ModelDesc, ModelData>
    {
    public:
        Model() = default;

        void Init(const Identifier &ident, ModelDesc *pDesc, ModelData *pData) override;
        static void ParseToMemory(ModelData *outData, BufferStream &modelBuffer);

        void AddSphere(float radius, u32 tessellation, TextureHandle texture);

        void Render();

        static constexpr ResourceType m_ResType = ResourceType::Model;

    private:
        std::vector<Mesh> m_Meshes;

        u32 m_IndexCount = 0;
    };

}  // namespace Lorr