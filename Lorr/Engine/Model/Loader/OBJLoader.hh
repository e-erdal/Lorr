//
// Created on Thursday 11th November 2021 by e-erdal
//

#pragma once

#include "ModelLoader.hh"

namespace lr::ModelLoader
{
    class OBJLoader : public IModelLoader
    {
    public:
        bool LoadBinary(ModelData *modelData, BufferStream &buf) override;
    };

}  // namespace lr::ModelLoader