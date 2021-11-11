#include "Model.hh"

#include "Loader/ModelLoader.hh"

namespace Lorr
{
    void Model::Init(const Identifier &ident, ModelDesc *pDesc, ModelData *pData)
    {
        ZoneScoped;

        for (auto &mesh : pData->Meshes)
        {
            Mesh staticMesh;
            staticMesh.Init(mesh.Vertices, mesh.Indices);
            m_Meshes.push_back(staticMesh);
        }
    }

    void Model::ParseToMemory(ModelData *outData, BufferStream &modelBuffer)
    {
        ModelLoader::ModelLoaderType type = ModelLoader::ModelLoaderType::OBJ;
        ModelLoader::LoadModel(type, outData, modelBuffer);
    }

    void Model::AddSphere(float radius, u32 tessellation, TextureHandle texture)
    {
        ZoneScoped;

        Mesh mesh;
        mesh.Init(radius, tessellation, texture);
        m_Meshes.push_back(mesh);
    }

    void Model::Render()
    {
        ZoneScoped;

        for (auto &mesh : m_Meshes) mesh.Render();
    }

}  // namespace Lorr