#include "Model.hh"

namespace Lorr
{
    void Model::Init(const std::string &path)
    {
        QuickOBJLoader::ModelResult result = QuickOBJLoader::LoadFromFile(path);

        std::vector<MeshVertex> vertexArr;
        std::vector<uint32_t> indexArr;

        for (auto &o : result.meshes)
        {
            QuickOBJLoader::MaterialResult matResult = QuickOBJLoader::LoadMaterialFromFile(o.materialPath);

            QuickOBJLoader::Material mat = {};
            for (auto &m : matResult.materials)
                if (m.Name == o.materialName) mat = m;

            Mesh mesh;
            mesh.Init(o.vertexData, o.indexData.size(), o.format, mat);

            m_Meshes.push_back(mesh);

            // Insert vertices
            const auto &mesVertices = mesh.GetVertices();
            vertexArr.resize(vertexArr.size() + mesVertices.size());
            MeshVertex *pVertices = &vertexArr[vertexArr.size() - mesVertices.size()];

            for (const auto &vertex : mesVertices)
            {
                *pVertices = vertex;
                pVertices++;
            }

            // Indert indices
            indexArr.resize(indexArr.size() + o.indexData.size());
            uint32_t *pIndices = &indexArr[indexArr.size() - o.indexData.size()];

            for (const auto &index : o.indexData)
            {
                *pIndices = index;
                pIndices++;
            }

            m_IndexCount += o.indexData.size();
        }

        m_VertexBuffer = RenderBuffer::Create(&vertexArr[0], vertexArr.size() * sizeof(MeshVertex), RenderBufferType::Vertex,
                                              RenderBufferUsage::Dynamic, RB_ACCESS_TYPE_CPUW);
        m_IndexBuffer = RenderBuffer::Create(&indexArr[0], indexArr.size() * sizeof(uint32_t), RenderBufferType::Index);
    }

}  // namespace Lorr