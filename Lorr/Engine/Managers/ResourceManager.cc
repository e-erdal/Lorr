#include "ResourceManager.hh"

namespace Lorr
{
    /// PARSERS

    Texture2D *ResourceManager::ParseTexture(const Identifier &ident, TEXTURE2D_DESC *pDesc, const std::string &path)
    {
        BufferStream buf;
        if (!ResourceManager::LoadFile(path, buf))
        {
            LOG_WARN("Failed to load Texture2D, continue as placeholder.");
            return 0;
        }
        
        buf.StartOver();
        TEXTURE2D_DESC_SUBRESC subresc = {
            .Width = buf.Get<uint32_t>(),
            .Height = buf.Get<uint32_t>(),
            .Format = (bgfx::TextureFormat::Enum)buf.Get<uint32_t>(),
            .DataSize = buf.Get<uint32_t>(),
        };

        subresc.Data = buf.GetPtr<uint8_t>(subresc.DataSize);

        Texture2D *handle = new Texture2D;
        handle->Init(ident, pDesc, &subresc);
        return handle;
    }

    /// -------

    void ResourceManager::Init()
    {
    }

    bool ResourceManager::LoadFile(const std::string &path, BufferStream &buf)
    {
        FileStream f(path, false);
        if (!f.IsOK())
        {
            return false;
        }

        uint8_t *fileData = f.ReadAll<uint8_t>();
        size_t fileLen = f.Size();

        f.Close();

        if (fileLen < sizeof(ResourceHeader))
        {
            LOG_WARN("Tried to load invalid resource file, it does not contain resource header.");
            return false;
        }

        BufferStream rawBuf(fileData, fileLen);
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

        free(fileData);

        return true;
    }

}  // namespace Lorr