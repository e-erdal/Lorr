//
// Created on Friday 24th September 2021 by e-erdal
// Main implementation by Mempler
//

#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "Engine/Stream/BufferStream.hh"
#include "Engine/Core/FileSystem.hh"

#include "Audio.hh"
#include "AudioChannel.hh"

namespace Lorr
{
    class AudioSystem
    {
    public:
        ~AudioSystem();

        void Init();

        void CreateChannel(Identifier const &ident, AudioChannel *pChannel);

        void LoadAudio(Identifier const &ident, Audio *pAudioOut, AudioChannel *pChannel, AudioData *pData);

    private:
        std::vector<AudioChannel *> m_AudioChannels;

        ALCdevice *m_alDevice;    // Active Audio Device
        ALCcontext *m_alContext;  // Active OpenAL Context (once per thread)
    };

}  // namespace Lorr
