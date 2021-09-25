#include "AudioChannel.hh"

namespace Lorr
{
    void AudioChannel::Init(const Identifier &ident)
    {
        m_Identifier = ident;
    }

    void AudioChannel::Add(Audio *pAudio)
    {
        pAudio->SetVolume(m_Volume);
        m_LinkedAudio.emplace_back(pAudio);
    }

    float AudioChannel::GetVolume()
    {
        return m_Volume;
    }

    void AudioChannel::SetVolume(float volume)
    {
        m_Volume = volume;

        for (auto audio : m_LinkedAudio)
        {
            audio->SetVolume(volume);  // Each audio file should get the volume this channel by default.
        }
    }

}  // namespace Lorr
