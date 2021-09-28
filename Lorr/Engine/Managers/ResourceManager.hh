//
// Created on Wednesday 22nd September 2021 by e-erdal
// A Resource Management system i really dont like its design
//

#pragma once

#include "Engine/Resource/IResource.hh"
#include "Engine/Resource/ResourceFile.hh"

#include "Engine/Stream/BufferStream.hh"
#include "Engine/Stream/FileStream.hh"

#include "Engine/Audio/AudioSystem.hh"
#include "Engine/Graphics/Shader.hh"
#include "Engine/Graphics/Texture2D.hh"

namespace Lorr
{
    class ResourceManager
    {
    public:
        ResourceManager() = default;

        void Init();

        // Export original data to something that Lorr can read
        bool ExportResource(ResourceType type, const std::string &path, BufferStream &buf);
        //
        bool ImportAudioData(const std::string &path, AudioData &outData);
        bool ImportShaderData(const std::string &path, ShaderData &outData);

    public:
        static bool LoadResourceFile(const std::string &path, BufferStream &buf);

    private:
        bool ParseToBuffer(ResourceType type, const std::string &path, BufferStream &outBuf);

        // In
        bool ParseTextureDataFromFile(Texture2DData &outData, BufferStream &resourceBuf);
        bool ParseAudioDataFromFile(AudioData &outData, BufferStream &resourceBuf);
        bool ParseShaderDataFromFile(ShaderData &outData, BufferStream &resourceBuf);

        // Out
        bool ParseTextureToBuffer(BufferStream &inBuf, BufferStream &outBuf);
        bool ParseAudioToBuffer(BufferStream &inBuf, BufferStream &outBuf);
        bool ParseShaderToBuffer(BufferStream &inBuf, BufferStream &outBuf);

    private:
        std::unordered_map<Identifier, IResource *> m_Resources;
    };

}  // namespace Lorr