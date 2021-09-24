#include "AudioChannel.hh"

namespace Lorr
{
    void AudioChannel::Init(const Identifier &ident)
    {
        m_Identifier = ident;
    }

    // void AudioChannel::CreateAudio(Audio *pAudioOut, uint32_t hSrcLeft, uint32_t hSrcRight, uint32_t hBufferLeft, uint32_t hBufferRight)
    // {
    //     pAudioOut->Init(this, hSrcLeft, hSrcRight, hBufferLeft, hBufferRight);

    //     m_LinkedAudio.emplace_back(pAudioOut);
    // }

    float AudioChannel::GetVolume()
    {
        return m_Volume;
    }
    void AudioChannel::SetVolume(float volume)
    {
        m_Volume = volume;

        // for (auto audio : m_vLinkedAudio)
        // {
        //     audio->SetVolume(fVolume);  // Each audio file should get the volume this channel by default.
        // }
    }

}  // namespace Lorr
