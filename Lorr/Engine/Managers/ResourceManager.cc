#include "ResourceManager.hh"

namespace Lorr
{
    void ResourceManager::Init()
    {
    }

    bool ResourceManager::ExportResource(ResourceType type, const std::string &path, BufferStream &buf)
    {
        BufferStream compBuf;
        if (!ParseToBuffer(type, path, compBuf)) return false;

        ResourceHeader header;
        header.Type = type;
        header.Flags |= RESOURCE_FILE_FLAGS_COMPRESSED;
        header.OriginalSize = compBuf.GetSize();

        compBuf.Compress();
        buf.Insert(header);
        buf.InsertPtr(compBuf.GetData(), compBuf.GetSize());

        return true;
    }

    bool ResourceManager::ImportAudioData(const std::string &path, AudioData &outData)
    {
        BufferStream resourceBuf;
        if (!ResourceManager::LoadResourceFile(path, resourceBuf))
        {
            LOG_WARN("Failed to load Audio resource.");
            return false;
        }

        resourceBuf.StartOver();

        return ParseAudioDataFromFile(outData, resourceBuf);
    }

    bool ResourceManager::ParseToBuffer(ResourceType type, const std::string &path, BufferStream &outBuf)
    {
        BufferStream fileBuf;
        if (!FileSystem::ReadBinaryFile(path, fileBuf))
        {
            LOG_WARN("Failed to load Texture2D, continue as placeholder.");
            return false;
        }

        fileBuf.StartOver();

        switch (type)
        {
            case ResourceType::Texture: return ParseTextureToBuffer(fileBuf, outBuf);
            case ResourceType::Audio: return ParseAudioToBuffer(fileBuf, outBuf);
            default: return false;
        }
    }

    bool ResourceManager::LoadResourceFile(const std::string &path, BufferStream &buf)
    {
        BufferStream rawBuf;
        if (!FileSystem::ReadBinaryFile(path.data(), rawBuf))
        {
            LOG_ERROR("Unable to load file {}.", path.c_str());
            return false;
        }

        if (rawBuf.GetSize() < sizeof(ResourceHeader))
        {
            LOG_WARN("Tried to load invalid resource file, it does not contain resource header.");
            return false;
        }

        auto header = rawBuf.Get<ResourceHeader>();
        size_t newSize = rawBuf.GetSize() - rawBuf.GetOffset();
        buf.InsertPtr(rawBuf.GetPtr<uint8_t>(newSize), newSize);

        // Validate resource file
        if (header.Signature != kResourceFileSignature)
        {
            LOG_WARN("Attempted to load invalid resource file. File signature does not match.");
            return false;
        }
        if (header.EngineVersion != ENGINE_VERSION_PACKED) LOG_WARN("Engine version does not match. But we will let the parser handle this.");
        if (header.Version < kResourceMinVersion)
        {
            LOG_WARN("Attempted to load outdated resource. Min: {}, req: {}.", kResourceMinVersion, header.Version);
            return false;
        }
        // !NOTE: Encryption has to be first.
        if (header.Flags & RESOURCE_FILE_FLAGS_COMPRESSED)
        {
            LOG_INFO("Decompressing file...");
            buf.Decompress(header.OriginalSize);
        }

        return true;
    }

    /// PARSERS

    bool ResourceManager::ParseTextureDataFromFile(Texture2DData &outData, BufferStream &resourceBuf)
    {
        resourceBuf.StartOver();
        outData.Width = resourceBuf.Get<uint32_t>();
        outData.Height = resourceBuf.Get<uint32_t>();
        outData.Format = (bgfx::TextureFormat::Enum)resourceBuf.Get<uint32_t>();
        outData.DataSize = resourceBuf.Get<uint32_t>();
        outData.Data = resourceBuf.GetPtr<uint8_t>(outData.DataSize);

        return true;
    }

    bool ResourceManager::ParseAudioDataFromFile(AudioData &outData, BufferStream &resourceBuf)
    {
        resourceBuf.StartOver();
        outData.PCMFrequency = resourceBuf.Get<uint32_t>();
        outData.PCMFormat = resourceBuf.Get<uint32_t>();
        uint32_t frameCount = resourceBuf.Get<uint32_t>();
        outData.PCMFrames.Reset(resourceBuf.GetPtr<uint8_t>(frameCount), frameCount);

        return true;
    }

    bool ResourceManager::ParseTextureToBuffer(BufferStream &inBuf, BufferStream &buf)
    {
        Texture2DData data;
        Texture2D::ParseMemory(&data, inBuf);

        buf.Reset(sizeof(Texture2DData) - sizeof(size_t) + data.DataSize);
        buf.Assign(data.Width);
        buf.Assign(data.Height);
        buf.Assign(data.Format);
        buf.Assign(data.DataSize);
        buf.AssignPtr(data.Data, data.DataSize);

        return true;
    }

    bool ResourceManager::ParseAudioToBuffer(BufferStream &inBuf, BufferStream &buf)
    {
        AudioData data;
        Audio::ParseToMemory(&data, inBuf);

        buf.Reset();
        buf.Insert(data.PCMFrequency);
        buf.Insert(data.PCMFormat);
        buf.Insert<uint32_t>(data.PCMFrames.GetSize());
        buf.InsertPtr(data.PCMFrames.GetData(), data.PCMFrames.GetSize());

        return true;
    }

    /// -------

}  // namespace Lorr