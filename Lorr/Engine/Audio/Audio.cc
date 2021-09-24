#include "Audio.hh"

#include "AudioChannel.hh"
#include "Helper.hh"

#include <AL/al.h>
#include <AL/alc.h>

namespace Lorr
{
    void Audio::Init(AudioChannel *pChannel, uint32_t srcLeft, uint32_t srcRight, uint32_t bufferLeft, uint32_t bufferRight)
    {
        m_pChannel = pChannel;
        m_SourceLeft = srcLeft;
        m_BufferLeft = bufferLeft;
        m_SourceRight = srcRight;
        m_BufferRight = bufferRight;
    }

    void Audio::Play()
    {
        alSourcePlay(m_SourceLeft);
        alSourcePlay(m_SourceRight);

        alSourcei(m_SourceLeft, AL_SOURCE_RELATIVE, AL_TRUE);
        alSourcei(m_SourceRight, AL_SOURCE_RELATIVE, AL_TRUE);

        alSource3f(m_SourceLeft, AL_POSITION, -1.0f, 0.0f, 0.0f);
        alSource3f(m_SourceRight, AL_POSITION, 1.0f, 0.0f, 0.0f);

        // Reapply Pitch / Volume
        SetPitch(m_Pitch);
        SetVolume(m_Volume);

        AL_ERROR_CHECK();
    }
}  // namespace Lorr