//
// Created on Friday 17th September 2021 by e-erdal
//

#pragma once

#include "Engine/Graphics/Shader.hh"
#include "Engine/Stream/FileStream.hh"
#include "Engine/Stream/BufferStream.hh"

#include <array>

const std::unordered_map<std::string, bgfx::RendererType::Enum> kRenderers = {
    { "glc", bgfx::RendererType::OpenGL },    { "met", bgfx::RendererType::Metal },      { "spv", bgfx::RendererType::Vulkan },
    { "vs3", bgfx::RendererType::Direct3D9 }, { "vs4", bgfx::RendererType::Direct3D11 }, { "vs5", bgfx::RendererType::Direct3D12 },
};

class ShaderCompiler
{
public:
    ShaderCompiler(const std::string &path, const std::string &out)
    {
        std::vector<Lorr::FileShader> shaders;

        for (const auto &r : kRenderers)
        {
            printf("Selected renderer: %s\n", bgfx::getRendererName((bgfx::RendererType::Enum)r.second));

            Lorr::FileStream vf(path + ".v." + r.first, false);
            if (vf.IsOK())
            {
                printf("Reading %s.v.%s\n", path.c_str(), r.first.c_str());

                Lorr::FileShader shader;
                shader.Renderer = r.second;
                shader.Type = Lorr::ShaderType::Vertex;
                shader.pData = vf.ReadAll<uint8_t>();
                shader.Len = vf.Size();
                shaders.push_back(shader);

                vf.Close();
            }

            Lorr::FileStream ff(path + ".f." + r.first, false);
            if (ff.IsOK())
            {
                printf("Reading %s.f.%s\n", path.c_str(), r.first.c_str());

                Lorr::FileShader shader;
                shader.Renderer = r.second;
                shader.Type = Lorr::ShaderType::Fragment;
                shader.pData = ff.ReadAll<uint8_t>();
                shader.Len = ff.Size();
                shaders.push_back(shader);

                ff.Close();
            }

            Lorr::FileStream cf(path + ".c." + r.first, false);
            if (cf.IsOK())
            {
                printf("Reading %s.c.%s\n", path.c_str(), r.first.c_str());

                Lorr::FileShader shader;
                shader.Renderer = r.second;
                shader.Type = Lorr::ShaderType::Compute;
                shader.pData = cf.ReadAll<uint8_t>();
                shader.Len = cf.Size();
                shaders.push_back(shader);

                cf.Close();
            }
        }

        Lorr::BufferStream buf;
        buf.Insert<uint8_t>(shaders.size());

        for (const auto &s : shaders)
        {
            buf.Insert(s.Type);
            buf.Insert<uint8_t>(s.Renderer);

            Lorr::BufferStream shaderBuf(s.pData, s.Len);
            shaderBuf.Compress();
            const auto &len = shaderBuf.GetSize();

            buf.Insert(s.Len);
            buf.Insert(len);
            buf.Expand(len);
            buf.AssignPtr(shaderBuf.GetData(), len);

            printf("%d, %d(%s), %d\n", (int)s.Type, s.Renderer, bgfx::getRendererName((bgfx::RendererType::Enum)s.Renderer), s.Len);
        }

        Lorr::FileStream w(out, true);
        w.WritePtr(buf.GetData(), buf.GetSize());
        w.Close();
    }
};