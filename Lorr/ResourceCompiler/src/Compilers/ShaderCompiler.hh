//
// Created on Friday 17th September 2021 by e-erdal
//

#pragma once

#include "Engine/App/Engine.hh"
#include "Engine/Graphics/Shader.hh"
#include "Engine/Stream/FileStream.hh"
#include "Engine/Stream/BufferStream.hh"

#include "Engine/Core/FileSystem.hh"

class ShaderCompiler
{
public:
    ShaderCompiler(const std::string &path, const std::string &out)
    {
        using namespace Lorr;

        printf("Loading Shader %s -> %s\n", path.c_str(), out.c_str());

        std::vector<ShaderInfo> shaders;
        bool hasVertex = false;
        bool hasFragment = false;
        bool hasCompute = false;

        for (const auto &r : kShaderRenderers)
        {
            FileStream vf(path + ".v." + r.first, false);
            if (vf.IsOK())
            {
                hasVertex = true;

                ShaderInfo shader;
                shader.Renderer = r.second;
                shader.Type = ShaderType::Vertex;
                shader.pData = vf.ReadAll<uint8_t>();
                shader.Len = vf.Size();
                shaders.push_back(shader);

                vf.Close();
            }

            FileStream ff(path + ".f." + r.first, false);
            if (ff.IsOK())
            {
                hasFragment = true;

                ShaderInfo shader;
                shader.Renderer = r.second;
                shader.Type = ShaderType::Fragment;
                shader.pData = ff.ReadAll<uint8_t>();
                shader.Len = ff.Size();
                shaders.push_back(shader);

                ff.Close();
            }

            FileStream cf(path + ".c." + r.first, false);
            if (cf.IsOK())
            {
                hasCompute = true;

                ShaderInfo shader;
                shader.Renderer = r.second;
                shader.Type = ShaderType::Compute;
                shader.pData = cf.ReadAll<uint8_t>();
                shader.Len = cf.Size();
                shaders.push_back(shader);

                cf.Close();
            }
        }

        BufferStream buf;
        buf.Insert<uint8_t>(hasVertex + hasFragment + hasCompute);

        for (const auto &s : shaders)
        {
            printf("Renderer: %s(%d), type: %d\n", bgfx::getRendererName(s.Renderer), s.Renderer, (int)s.Type);
            buf.Insert<uint8_t>(s.Renderer);
            buf.Insert(s.Type);
            buf.Insert(s.Len);
            buf.InsertPtr(s.pData, s.Len);
        }

        FileSystem::WriteBinaryFile(out, buf);
        if (!GetEngine()->GetResourceMan()->ExportResource(ResourceType::Shader, out, buf)) LOG_ERROR("Unable to compile resource.");
        FileSystem::WriteBinaryFile(out, buf);
    }
};