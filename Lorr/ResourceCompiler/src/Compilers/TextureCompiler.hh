//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "Engine/App/Engine.hh"
#include "Engine/Resource/ResourceFile.hh"
#include "Engine/Graphics/Texture2D.hh"

#include "Engine/Stream/FileStream.hh"
#include "Engine/Stream/BufferStream.hh"

#include "Engine/Core/FileSystem.hh"

class TextureCompiler
{
public:
    TextureCompiler(const std::string &path, const std::string &out)
    {
        using namespace Lorr;

        printf("Loading Texture2D %s -> %s\n", path.c_str(), out.c_str());

        BufferStream buf;
        GetEngine()->GetResourceMan()->ExportResource(ResourceType::Texture, path, buf);
        FileSystem::WriteBinaryFile(out, buf);
    }
};