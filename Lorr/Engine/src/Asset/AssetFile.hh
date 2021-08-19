//
// Created on August 17th 2021 by e-erdal.
//

#pragma once

namespace Lorr
{
    constexpr uint32_t FileSignature = 1920102220;

    struct AssetHeader
    {
        uint32_t Signature = FileSignature;
        uint16_t Version = 1;
        uint16_t MinVersion = 1;
        uint32_t EngineVersion = 0xffffffff;
        uint32_t GameVersion = 0xffffffff;
    };

}  // namespace Lorr