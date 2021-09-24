//
// Created on Friday 24th September 2021 by e-erdal
// Main implementation by Mempler
//

#pragma once

#include "Engine/Stream/BufferStream.hh"

namespace Lorr
{
    struct AudioData
    {
        bool Mono = false;
        
        BufferStream LeftBuffer;
        BufferStream RightBuffer;
    };

}  // namespace Lorr
