//
// Created on Friday 24th September 2021 by e-erdal
// Main implementation by Mempler
//

#pragma once

#include "Engine/Resource/IResource.hh"

namespace Lorr
{
    class AudioChannel;
    
    struct AudioDesc
    {
        AudioChannel *pChannel = 0;
        // TODO: Settings/info for effects, etc...
    };

    struct AudioData
    {
        uint32_t PCMFrequency;
        uint32_t PCMFormat;

        BufferStream PCMFrames;
    };

    class Audio : public IResource<AudioDesc, AudioData>
    {
    public:
        Audio() = default;

        void Init(const Identifier &ident, AudioDesc *pDesc, AudioData *pData) override;
        static void ParseToMemory(AudioData *outData, BufferStream &audioBuffer);

        void Play();
        void Stop();
        void Pause();
        void Destroy();

        void SetLooping(bool loop);

        float GetPitch();
        void SetPitch(float pitch);

        float GetVolume();
        void SetVolume(float volume);

        void SetPosition(glm::vec3 position);

        static constexpr ResourceType m_ResType = ResourceType::Audio;

    private:
        AudioChannel *m_pChannel;

        float m_Volume = 1.0f;
        float m_Pitch = 1.0f;

        uint32_t m_Buffer;
        uint32_t m_Source;

        bool m_Looping = false;

        void __SetVolume(float volume);
    };

}  // namespace Lorr