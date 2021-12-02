//
// Created on Wednesday 22nd September 2021 by e-erdal
// A Resource Management system i really dont like its design
//

#pragma once

#include "Engine/Resource/IResource.hh"
#include "Engine/Resource/ResourceFile.hh"

#include "Engine/Stream/BufferStream.hh"
#include "Engine/Stream/FileStream.hh"

#include "Engine/Graphics/Common/BaseTexture.hh"
#include "Engine/Graphics/Common/BaseShader.hh"

#include "Engine/Audio/AudioSystem.hh"
#include "Engine/Graphics/Font.hh"
#include "Engine/Model/Model.hh"

namespace Lorr
{
    class ResourceManager
    {
    public:
        ResourceManager() = default;

        void Init();

        template<typename T>
        inline T *GetResource(const Identifier &ident)
        {
            ZoneScoped;

            auto found = m_Resources.find(ident);
            return (found == m_Resources.end() ? 0 : (T *)found->second);
        }

        template<typename T>
        inline T *LoadResource(const Identifier &ident, const std::string &path, decltype(T::m_DescType) *pDesc)
        {
            ZoneScoped;

            decltype(T::m_DataType) data;
            if (!ImportResource(T::m_ResType, path, data)) return 0;

            T *resource = new T;
            resource->Init(ident, pDesc, &data);
            m_Resources.emplace(ident, resource);

            return resource;
        }

        template<typename T>
        inline bool ImportResource(ResourceType type, const std::string &path, T &outData)
        {
            ZoneScoped;

            BufferStream resourceBuf;
            if (!ResourceManager::LoadResourceFile(path, resourceBuf))
            {
                LOG_WARN("Failed to load resource file.");
                return false;
            }

            resourceBuf.StartOver();

            switch (type)
            {
                case ResourceType::Texture: return ParseTextureDataFromFile((TextureData &)outData, resourceBuf);
                case ResourceType::Audio: return ParseAudioDataFromFile((AudioData &)outData, resourceBuf);
                case ResourceType::Shader: return ParseShaderDataFromFile((ShaderData &)outData, resourceBuf);
                case ResourceType::Font: return ParseFontDataFromFile((FontData &)outData, resourceBuf);
                case ResourceType::Model: return ParseModelDataFromFile((ModelData &)outData, resourceBuf);
                default: return false;
            }

            return false;
        }

        bool ExportResource(ResourceType type, const std::string &path, BufferStream &buf);

        template<typename T>
        inline void Append(T *pResource)
        {
            m_Resources.emplace(pResource->GetIdentifier(), pResource);
        }

    public:
        static bool LoadResourceFile(const std::string &path, BufferStream &buf);

    private:
        bool ParseToBuffer(ResourceType type, const std::string &path, BufferStream &outBuf);

        // In
        bool ParseTextureDataFromFile(TextureData &outData, BufferStream &resourceBuf);
        bool ParseAudioDataFromFile(AudioData &outData, BufferStream &resourceBuf);
        bool ParseShaderDataFromFile(ShaderData &outData, BufferStream &resourceBuf);
        bool ParseFontDataFromFile(FontData &outData, BufferStream &resourceBuf);
        bool ParseModelDataFromFile(ModelData &outData, BufferStream &resourceBuf);

        // Out
        bool ParseTextureToBuffer(BufferStream &inBuf, BufferStream &outBuf);
        bool ParseAudioToBuffer(BufferStream &inBuf, BufferStream &outBuf);
        bool ParseShaderToBuffer(BufferStream &inBuf, BufferStream &outBuf);
        bool ParseFontToBuffer(BufferStream &inBuf, BufferStream &outBuf);
        bool ParseModelToBuffer(BufferStream &inBuf, BufferStream &outBuf);

    private:
        std::unordered_map<Identifier, void *> m_Resources;
    };

}  // namespace Lorr