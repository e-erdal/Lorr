//
// Created on August 16th 2021 by e-erdal.
//

#pragma once

#define C_RTFILE_PACKAGE_HEADER_BYTE_SIZE 6

#include "Core/Graphics/Texture/Texture2D.hh"

struct RTFileHeader
{
    uint8_t fileTypeID[C_RTFILE_PACKAGE_HEADER_BYTE_SIZE];
    uint8_t version;
    uint8_t reserved[1];
};

struct RTPackHeader
{
    RTFileHeader rtFileHeader;
    uint32_t compressedSize;
    uint32_t decompressedSize;
    uint8_t compressionType;  // one of eCompressionType
    uint8_t reserved[15];
};

struct RTTextureHeader
{
    int height;
    int width;
    int format;
    int originalh;
    int originalw;
    bool alpha;
    bool compressed;
    short flags;
    int mipmaps;
    int reserved[16];
};

namespace RTTEX
{
    Lorr::Texture2D *ToTexture(const Lorr::Identifier &ident, const std::string &path);
}