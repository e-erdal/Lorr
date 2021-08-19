#include "RTTexture.hh"

#include "Core/Stream/BufferStream.hh"
#include "Core/Stream/FileStream.hh"

using namespace Lorr;

Texture2D *RTTEX::ToTexture(const Lorr::Identifier &ident, const std::string &path)
{
    ZoneScoped;

    Console::Info("Loading RTTexture '{}'...", path);

    FileStream file(path, false);

    if (!file.IsOK())
    {
        Console::Fatal("Failed to load file '{}'!", path);
        return 0;
    }

    uint8_t *pData = file.ReadAll<uint8_t>();
    size_t dataSize = file.Size();

    BufferStream buffer(pData, dataSize);

    if (buffer.GetSize() < (sizeof(RTTextureHeader) + sizeof(RTFileHeader)))
    {
        Console::Error("Failed to load texture {}! File size is smaller than expected.", path);

        free(pData);

        return 0;
    }

    RTPackHeader header = buffer.Get<RTPackHeader>();
    buffer.Decompress(header.compressedSize, header.decompressedSize);
    buffer.Get<char>(8);
    RTTextureHeader textureHeader = buffer.Get<RTTextureHeader>();
    buffer.Get<char>(24);

    size_t textureDataSize = textureHeader.width * textureHeader.height * (textureHeader.alpha ? 4 : 3);

    TEXTURE2D_DESC texDesc;
    texDesc.Filters = TEXTURE_MAG_NEAREST;

    TEXTURE2D_DESC_SUBRESC srDesc;
    srDesc.Width = textureHeader.width;
    srDesc.Height = textureHeader.height;
    srDesc.Format = bgfx::TextureFormat::BGRA8;
    srDesc.Data = buffer.Get<uint8_t *>(textureDataSize);
    srDesc.DataSize = textureDataSize;

    Texture2D *texture = new Texture2D;
    texture->Init(ident, &texDesc, &srDesc);

    free(pData);

    return texture;
}