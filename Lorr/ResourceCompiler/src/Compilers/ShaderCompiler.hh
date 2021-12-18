//
// Created on Friday 17th September 2021 by e-erdal
//

#pragma once

#include "Engine/App/Engine.hh"

#include "Engine/Stream/FileStream.hh"
#include "Engine/Stream/BufferStream.hh"

#include "Engine/Core/FileSystem.hh"

class ShaderCompiler
{
public:
    ShaderCompiler(const std::string &path, const std::string &out)
    {
        using namespace lr;

        BufferStream buf;

        printf("Loading Vertex Shader %s -> %s\n", path.c_str(), out.c_str());
        GetEngine()->GetResourceMan()->ExportResource(ResourceType::Shader, path, buf);
        FileSystem::WriteBinaryFile(out, buf);
    }
};