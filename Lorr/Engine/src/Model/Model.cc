#include "Model.hh"

namespace Lorr
{
    Model::~Model()
    {
    }

    void Model::Init( const char *szPath )
    {
        QuickOBJLoader::ModelResult res = QuickOBJLoader::LoadFromFile( szPath );

        for ( auto &o : res.meshes )
        {
            QuickOBJLoader::MaterialResult mres = QuickOBJLoader::LoadMaterialFromFile( o.materialPath );

            QuickOBJLoader::Material mat{};
            for ( auto &m : mres.materials )
                if ( m.Name == o.materialName ) mat = m;

            Mesh mesh;
            mesh.Init( o.vertexData, o.indexData, o.format, mat );

            m_vMeshes.push_back( mesh );
        }
    }

    void Model::Render()
    {
        for ( auto &mesh : m_vMeshes ) mesh.Render();
    }

}  // namespace Lorr