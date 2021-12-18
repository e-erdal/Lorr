//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

#include "IResource.hh"

namespace lr
{
    constexpr u32 kResourceFileSignature = 1920102220;
    constexpr u16 kResourceMinVersion = 1;

    enum
    {
        RESOURCE_FILE_FLAGS_NONE = 0,
        RESOURCE_FILE_FLAGS_COMPRESSED = 1 << 0,
    };

#pragma pack(push, 1)
    struct ResourceHeader
    {
        u32 Signature = kResourceFileSignature;
        u16 Version = 1;
        u32 EngineVersion = ENGINE_VERSION_PACKED;
        u32 GameVersion = 0xffffffff;
        u32 OriginalSize = 0xffffffff;
        u32 Flags = 0;
        ResourceType Type = ResourceType::Invalid;
    };
#pragma pack(pop)

}  // namespace lr