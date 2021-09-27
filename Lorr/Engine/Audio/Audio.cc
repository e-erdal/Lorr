#include "Audio.hh"

#include <AL/al.h>

#include "Helper.hh"

#include "AudioChannel.hh"
#include "Loader/AudioLoader.hh"

namespace Lorr
{
    void Audio::Init(const Identifier &ident, AudioChannel *pChannel)
    {
        m_Ident = ident;
        m_pChannel = pChannel;
    }

    void Audio::InitFromMemory(AudioData *pData)
    {
        alGenSources(1, &m_Source);
        alGenBuffers(1, &m_Buffer);
        alBufferData(m_Buffer, pData->PCMFormat, pData->PCMFrames.GetData(), pData->PCMFrames.GetSize(), pData->PCMFrequency);
        alSourcei(m_Source, AL_BUFFER, m_Buffer);

        AL_ERROR_CHECK();
        if (AL_IS_ERROR())
        {
            if (m_Buffer && alIsBuffer(m_Buffer)) alDeleteBuffers(1, &m_Buffer);
            if (m_Source && alIsSource(m_Source)) alDeleteSources(1, &m_Source);
            return;
        }
    }

    void Audio::ParseToMemory(AudioData *outData, BufferStream &audioBuffer)
    {
        const uint32_t kWavSig = 1179011410;
        const uint32_t kOggSig = 1399285583;

        uint32_t curSig = audioBuffer.Get<uint32_t>();
        audioBuffer.StartOver();

        AudioLoader::AudioLoaderType type;

        if (curSig == kWavSig)
            type = AudioLoader::AudioLoaderType::WAV;
        else if (curSig == kOggSig)
            type = AudioLoader::AudioLoaderType::OGG;
        else
            type = AudioLoader::AudioLoaderType::MP3;

        AudioLoader::LoadAudio(type, outData, audioBuffer);
    }

    void Audio::Play()
    {
        alSourcePlay(m_Source);
        alSourcei(m_Source, AL_SOURCE_RELATIVE, AL_TRUE);
        alSource3f(m_Source, AL_POSITION, -1.0f, 0.0f, 0.0f);

        // Reapply Pitch / Volume
        SetPitch(m_Pitch);
        SetVolume(m_Volume);

        AL_ERROR_CHECK();
    }

    void Audio::Stop()
    {
        alSourceStop(m_Source);
    }

    void Audio::Pause()
    {
        alSourcePause(m_Source);
    }

    void Audio::SetLooping(bool loop)
    {
        m_Looping = loop;

        alSourcei(m_Source, AL_LOOPING, loop);
    }

    float Audio::GetPitch()
    {
        return m_Pitch;
    }

    void Audio::SetPitch(float pitch)
    {
        m_Pitch = pitch;

        alSourcef(m_Source, AL_PITCH, m_Pitch);
    }

    float Audio::GetVolume()
    {
        return m_Volume;
    }

    void Audio::SetVolume(float volume)
    {
        m_Volume = volume;

        alSourcef(m_Source, AL_GAIN, m_Volume * m_pChannel->GetVolume());
    }

    void Audio::SetPosition(glm::vec3 position)
    {
    }
}  // namespace Lorr