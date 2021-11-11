#include "ModelLoader.hh"

#include "OBJLoader.hh"

namespace Lorr
{
    bool ModelLoader::LoadModel(ModelLoaderType type, ModelData *modelData, BufferStream &buf)
    {
        switch (type)
        {
            case ModelLoaderType::OBJ: OBJLoader loader; return loader.LoadBinary(modelData, buf);
        }

        return false;
    }

}  // namespace Lorr