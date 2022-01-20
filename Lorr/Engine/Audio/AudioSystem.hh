//
// Created on Friday 24th September 2021 by e-erdal
// Main implementation by Mempler
//

#pragma once

#include "Engine/Utils/BufferStream.hh"
#include "Engine/Core/FileSystem.hh"

#include "Audio.hh"
#include "AudioChannel.hh"

class ALCdevice;
class ALCcontext;
namespace lr
{
    class AudioSystem
    {
    public:
        ~AudioSystem();

        void Init();
        void CreateChannel(Identifier const &ident, AudioChannel *pChannel);

        void LoadAudio(Identifier const &ident, Audio *pAudioOut, AudioData *pData, const Identifier &channelIdent);

    private:
        eastl::unordered_map<Identifier, AudioChannel *> m_AudioChannels;

        ALCdevice *m_alDevice;    // Active Audio Device
        ALCcontext *m_alContext;  // Active OpenAL Context (once per thread)
    };

}  // namespace lr
