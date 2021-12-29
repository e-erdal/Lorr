#include "OBJLoader.hh"

#include <obj_loader.h>
#include <meshoptimizer.h>

namespace lr::ModelLoader
{
    bool OBJLoader::LoadBinary(ModelData *modelData, BufferStream &buf)
    {
        QuickOBJLoader::ModelResult result = QuickOBJLoader::LoadFromString(std::string_view((char *)buf.GetData(), buf.GetSize()));
        if (result.meshes.size() == 0) return false;

        for (auto &mesh : result.meshes)
        {
            FileMesh fileMesh;

            // fileMesh.Vertices.resize(mesh.vertexData.size() / (sizeof(FileMeshVertex) / sizeof(float)));
            // FileMeshVertex *pVertex = &fileMesh.Vertices[0];

            size_t offset = 0;
            while (offset != mesh.vertexData.size())
            {
                FileMeshVertex vertex{};
                if (mesh.format.positionElementOffset.has_value())
                {
                    float x = mesh.vertexData[offset + 0];
                    float y = mesh.vertexData[offset + 1];
                    float z = mesh.vertexData[offset + 2];
                    offset += 3;

                    vertex.Pos = { x, y, z };
                }

                if (mesh.format.normalElementOffset.has_value())
                {
                    float x = mesh.vertexData[offset + 0];
                    float y = mesh.vertexData[offset + 1];
                    float z = mesh.vertexData[offset + 2];
                    offset += 3;

                    vertex.Norm = { x, y, z };
                }

                if (mesh.format.textureCoordinatesElementOffset.has_value())
                {
                    float u = mesh.vertexData[offset + 0];
                    float v = mesh.vertexData[offset + 1];
                    offset += 2;

                    vertex.UV = { u, 1 - v };
                }

                fileMesh.Vertices.push_back(vertex);
            }

            fileMesh.Indices.resize(mesh.indexData.size());
            memcpy(&fileMesh.Indices[0], &mesh.indexData[0], mesh.indexData.size() * sizeof(u32));

            modelData->Meshes.push_back(fileMesh);
        }

        return true;
    }

}  // namespace lr::ModelLoader