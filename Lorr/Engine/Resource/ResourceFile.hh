//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "IResource.hh"

namespace Lorr
{
    constexpr uint32_t kResourceFileSignature = 1920102220;
    constexpr uint16_t kResourceMinVersion = 1;

    enum
    {
        RESOURCE_FILE_FLAGS_NONE = 0,
        RESOURCE_FILE_FLAGS_COMPRESSED = 1 << 0,
    };

#pragma pack(push, 1)
    struct ResourceHeader
    {
        uint32_t Signature = kResourceFileSignature;
        uint16_t Version = 1;
        uint32_t EngineVersion = ENGINE_VERSION_PACKED;
        uint32_t GameVersion = 0xffffffff;
        uint32_t OriginalSize = 0xffffffff;
        uint32_t Flags = 0;
        ResourceType Type = ResourceType::Invalid;
    };
#pragma pack(pop)

}  // namespace Lorr