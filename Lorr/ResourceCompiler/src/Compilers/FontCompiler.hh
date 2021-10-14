//
// Created on Wednesday 13th October 2021 by e-erdal
//

#pragma once

#include "Engine/App/Engine.hh"
#include "Engine/Resource/ResourceFile.hh"

#include "Engine/Stream/FileStream.hh"
#include "Engine/Stream/BufferStream.hh"

#include "Engine/Core/FileSystem.hh"

class FontCompiler
{
public:
    FontCompiler(const std::string &path, const std::string &out)
    {
        using namespace Lorr;

        printf("Loading Font %s -> %s\n", path.c_str(), out.c_str());

        BufferStream buf;
        GetEngine()->GetResourceMan()->ExportResource(ResourceType::Font, path, buf);
        FileSystem::WriteBinaryFile(out, buf);
    }
};