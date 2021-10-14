#include "ResourceManager.hh"

namespace Lorr
{
    void ResourceManager::Init()
    {
    }

    bool ResourceManager::ExportResource(ResourceType type, const std::string &path, BufferStream &buf)
    {
        buf.Reset();

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

    bool ResourceManager::ParseToBuffer(ResourceType type, const std::string &path, BufferStream &outBuf)
    {
        BufferStream fileBuf;
        if (!FileSystem::ReadBinaryFile(path, fileBuf))
        {
            LOG_WARN("Failed to load resource {}.", path.c_str());
            return false;
        }

        fileBuf.StartOver();

        switch (type)
        {
            case ResourceType::Texture: return ParseTextureToBuffer(fileBuf, outBuf);
            case ResourceType::Audio: return ParseAudioToBuffer(fileBuf, outBuf);
            case ResourceType::Shader: return ParseShaderToBuffer(fileBuf, outBuf);
            case ResourceType::Font: return ParseFontToBuffer(fileBuf, outBuf);
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
        if (header.EngineVersion != ENGINE_VERSION_PACKED)
            LOG_WARN("Engine version does not match. But we will let the parser handle this.");
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

    /// PARSERS -----------------------------------------------------------------------------------

    // ********************
    // * From memory section ----------------------------------------------------------------------
    // ********************

    bool ResourceManager::ParseTextureDataFromFile(TextureData &outData, BufferStream &resourceBuf)
    {
        resourceBuf.StartOver();
        outData.Width = resourceBuf.Get<uint32_t>();
        outData.Height = resourceBuf.Get<uint32_t>();
        outData.Format = resourceBuf.Get<TextureFormat>();
        outData.DataSize = resourceBuf.Get<uint32_t>();
        outData.Data = resourceBuf.GetPtrNew<uint8_t>(outData.DataSize);

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

    bool ResourceManager::ParseShaderDataFromFile(ShaderData &outData, BufferStream &resourceBuf)
    {
        resourceBuf.StartOver();
        outData.Renderer = resourceBuf.Get<APIType>();
        outData.Type = resourceBuf.Get<ShaderType>();
        uint32_t len = resourceBuf.Get<uint32_t>();
        outData.Buffer.Reset(resourceBuf.GetPtr<uint8_t>(len), len);

        return true;
    }

    bool ResourceManager::ParseFontDataFromFile(FontData &outData, BufferStream &resourceBuf)
    {
        outData.TTFData.Reset(resourceBuf);

        return true;
    }

    // ********************
    // * To memory section ------------------------------------------------------------------------
    // ********************

    bool ResourceManager::ParseTextureToBuffer(BufferStream &inBuf, BufferStream &outBuf)
    {
        TextureData data;
        ITexture::ParseToMemory(&data, inBuf);

        outBuf.Reset();
        outBuf.Insert(data.Width);
        outBuf.Insert(data.Height);
        outBuf.Insert(data.Format);
        outBuf.Insert(data.DataSize);
        outBuf.InsertPtr(data.Data, data.DataSize);

        return true;
    }

    bool ResourceManager::ParseAudioToBuffer(BufferStream &inBuf, BufferStream &outBuf)
    {
        AudioData data;
        Audio::ParseToMemory(&data, inBuf);

        outBuf.Reset();
        outBuf.Insert(data.PCMFrequency);
        outBuf.Insert(data.PCMFormat);
        outBuf.Insert((uint32_t)data.PCMFrames.GetSize());
        outBuf.InsertPtr(data.PCMFrames.GetData(), data.PCMFrames.GetSize());

        return true;
    }

    bool ResourceManager::ParseShaderToBuffer(BufferStream &inBuf, BufferStream &outBuf)
    {
        ShaderData data;
        IShader::ParseToMemory(&data, inBuf);

        outBuf.Reset();
        outBuf.Insert(data.Renderer);
        outBuf.Insert(data.Type);
        outBuf.Insert<uint32_t>(data.Buffer.GetSize());
        outBuf.Insert(data.Buffer);

        return true;
    }

    bool ResourceManager::ParseFontToBuffer(BufferStream &inBuf, BufferStream &outBuf)
    {
        FontData data;
        Font::ParseToMemory(&data, inBuf);

        outBuf.Reset(inBuf.GetData(), inBuf.GetSize());

        return true;
    }

    /// -------------------------------------------------------------------------------------------

}  // namespace Lorr