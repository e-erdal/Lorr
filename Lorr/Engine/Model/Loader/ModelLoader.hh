//
// Created on Thursday 11th November 2021 by e-erdal
//

#pragma once

#include "Engine/Model/Model.hh"
#include "Engine/Stream/BufferStream.hh"

namespace lr::ModelLoader
{
    enum class ModelLoaderType
    {
        OBJ,  // Wavefront
    };

    bool LoadModel(ModelLoaderType type, ModelData *modelData, BufferStream &buf);

    class IModelLoader
    {
    public:
        virtual bool LoadBinary(ModelData *modelData, BufferStream &buf) = 0;
    };
}  // namespace lr::ModelLoader