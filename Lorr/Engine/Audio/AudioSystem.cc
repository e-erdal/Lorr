#include "AudioSystem.hh"

#include "Helper.hh"

#include "Engine/App/Engine.hh"

namespace lr
{
    void AudioSystem::Init()
    {
        m_alDevice = alcOpenDevice(0);  // Select the default device

        if (!m_alDevice)
        {
            LOG_ERROR("Failed to create OpenAL device.");
        }

        m_alContext = alcCreateContext(m_alDevice, nullptr);
        if (m_alContext == nullptr || alcMakeContextCurrent(m_alContext) == ALC_FALSE)
        {
            if (m_alContext != nullptr) alcDestroyContext(m_alContext);

            alcCloseDevice(m_alDevice);

            LOG_ERROR("Failed set a Audio Context!");
        }

        const char *name = nullptr;

        if (alcIsExtensionPresent(m_alDevice, "ALC_ENUMERATE_ALL_EXT")) name = alcGetString(m_alDevice, ALC_ALL_DEVICES_SPECIFIER);
        if (!name || alcGetError(m_alDevice) != AL_NO_ERROR) name = alcGetString(m_alDevice, ALC_DEVICE_SPECIFIER);

        LOG_TRACE("Audio Device: {}", name);
        LOG_TRACE("Successfully Initialized Audio System!");
    }

    void AudioSystem::CreateChannel(Identifier const &ident, AudioChannel *pChannel)
    {
        pChannel->Init(ident);
        m_AudioChannels[ident] = pChannel;
    }

    void AudioSystem::LoadAudio(Identifier const &ident, Audio *pAudioOut, AudioData *pData, const Identifier &channelIdent)
    {
        AudioChannel *pTargetChannel = 0;
        auto itChannel = m_AudioChannels.find(channelIdent);

        if (itChannel == m_AudioChannels.end())
        {
            pTargetChannel = new AudioChannel;
            CreateChannel(channelIdent, pTargetChannel);
        }
        else
        {
            pTargetChannel = itChannel->second;
        }

        AudioDesc desc{ .pChannel = pTargetChannel };
        pAudioOut->Init(ident, &desc, pData);
    }

}  // namespace lr