//
// Created on Friday 24th September 2021 by e-erdal
// Main implementation by Mempler
//
// an Audio channel is the definition of a Master sound file
// basically you can adjust the volume, position etc etc for all sound files
// that are linked to this Channel

#pragma once

namespace Lorr
{
    class AudioChannel
    {
    public:
        AudioChannel() = default;

        void Init(const Identifier &ident);

        // void CreateAudio(Audio *pAudioOut, uint32_t hSrcLeft, uint32_t hSrcRight, uint32_t hBufferLeft, uint32_t hBufferRight);

        float GetVolume();
        void SetVolume(float volume);

    private:
        Identifier m_Identifier = "audiochannel://default";

        float m_Volume = 1.0f;

        // std::vector<Audio *> m_vLinkedAudio;
    };
    
}  // namespace Lorr