#include "AudioSystem.hh"

#include "Helper.hh"

#include "Engine/App/Engine.hh"

namespace Lorr
{
    void AudioSystem::Init()
    {
        m_alDevice = alcOpenDevice(nullptr);  // Select the default device

        CHECK(m_alDevice != nullptr, "Failed to open Audio Device!");

        m_alContext = alcCreateContext(m_alDevice, nullptr);
        if (m_alContext == nullptr || alcMakeContextCurrent(m_alContext) == ALC_FALSE)
        {
            if (m_alContext != nullptr) alcDestroyContext(m_alContext);

            alcCloseDevice(m_alDevice);

            CHECK(false, "Failed set a Audio Context!");
        }

        const char *name = nullptr;

        if (alcIsExtensionPresent(m_alDevice, "ALC_ENUMERATE_ALL_EXT")) name = alcGetString(m_alDevice, ALC_ALL_DEVICES_SPECIFIER);

        if (!name || alcGetError(m_alDevice) != AL_NO_ERROR) name = alcGetString(m_alDevice, ALC_DEVICE_SPECIFIER);

        LOG_INFO("Audio Device: {}", name);
        LOG_INFO("Successfully Initialized Audio System!");
    }

    void AudioSystem::CreateChannel(Identifier const &ident, AudioChannel *pChannel)
    {
        pChannel->Init(ident);
        m_AudioChannels.emplace_back(pChannel);
    }

    void AudioSystem::LoadAudio(Identifier const &ident, Audio *pAudioOut, AudioChannel *pChannel, AudioData *pData)
    {
        pAudioOut->Init(ident, pChannel);
        pAudioOut->InitFromMemory(pData);
        
        pChannel->Add(pAudioOut);
    }

}  // namespace Lorr