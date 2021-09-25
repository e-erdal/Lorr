//
// Created on Friday 24th September 2021 by e-erdal
// Main implementation by Mempler
//

#pragma once

#include "Engine/Stream/BufferStream.hh"
#include "Engine/Audio/Audio.hh"

namespace Lorr
{
    namespace AudioLoader
    {
        enum class AudioLoaderType
        {
            WAV,
            OGG,
            MP3
        };

        bool LoadAudio(AudioLoaderType type, AudioData *audioData, BufferStream &buf);

        class IAudioLoader
        {
        public:
            virtual bool LoadBinary(AudioData *audioData, BufferStream &buf) = 0;
        };

    }  // namespace AudioLoader
}  // namespace Lorr