//
// Created on Friday 24th September 2021 by e-erdal
// Main implementation by Mempler
//

#pragma once

namespace Lorr
{
    class AudioChannel;
    class Audio
    {
    public:
        Audio() = default;

        void Init(AudioChannel *pChannel, uint32_t srcLeft, uint32_t srcRight, uint32_t bufferLeft, uint32_t bufferRight);

        void Play();
        void Stop();
        void Pause();
        void Destroy();

        float GetPitch();
        void SetPitch(float pitch);

        float GetVolume();
        void SetVolume(float volume);

        void SetPosition(glm::vec3 position);

    private:
        AudioChannel *m_pChannel;

        float m_Volume = 1.0f;
        float m_Pitch = 1.0f;

        uint32_t m_BufferLeft;
        uint32_t m_BufferRight;

        uint32_t m_SourceLeft;
        uint32_t m_SourceRight;

        void __SetVolume(float volume);
    };

}  // namespace Lorr