#include "Model.hh"

namespace Lorr
{
    void Model::Init(const std::string &path)
    {
        ZoneScoped;

        LOG_TRACE("Loadimg model {}...", path);

        QuickOBJLoader::ModelResult result = QuickOBJLoader::LoadFromFile(path);

        std::vector<MeshVertex> vertexArr;
        std::vector<u32> indexArr;

        for (auto &o : result.meshes)
        {
            QuickOBJLoader::MaterialResult matResult = QuickOBJLoader::LoadMaterialFromFile(o.materialPath);

            QuickOBJLoader::Material mat = {};
            for (auto &m : matResult.materials)
                if (m.Name == o.materialName) mat = m;

            Mesh mesh;
            mesh.Init(o.vertexData, o.indexData, o.format, mat);

            m_Meshes.push_back(mesh);

            m_IndexCount += o.indexData.size();
        }
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