//
// Created on Friday 24th September 2021 by e-erdal
//

#pragma once 

#include "AudioLoader.hh"

namespace lr::AudioLoader
{
    class OggAudioLoader : public IAudioLoader
    {
    public:
        bool LoadBinary(AudioData *audioData, BufferStream &buf) override;
    };
    
}  // namespace lr::AudioLoader