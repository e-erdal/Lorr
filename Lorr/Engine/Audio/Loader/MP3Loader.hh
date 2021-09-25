//
// Created on Saturday 25th September 2021 by e-erdal
//

#pragma once

#include "AudioLoader.hh"

namespace Lorr::AudioLoader
{
    class MP3AudioLoader : public IAudioLoader
    {
    public:
        bool LoadBinary(AudioData *audioData, BufferStream &buf) override;
    };

}  // namespace Lorr::AudioLoader