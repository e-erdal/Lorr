#include "WavLoader.hh"

#include <AL/al.h>
#include <dr_wav.h>

namespace lr::AudioLoader
{
    bool WavAudioLoader::LoadBinary(AudioData *audioData, BufferStream &buf)
    {
        drwav wav;
        if (!drwav_init_memory(&wav, buf.GetData(), buf.GetSize(), nullptr))
        {
            LOG_WARN("Failed to load MP3 file.");
            return false;
        }

        size_t dataLen = wav.totalPCMFrameCount * wav.channels * wav.bitsPerSample;
        u8 *data = (u8 *)malloc(dataLen);

        drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, data);

        audioData->PCMFrames.Reset(data, dataLen);
        audioData->PCMFrequency = wav.sampleRate;

        switch (wav.channels)
        {
            case 1: audioData->PCMFormat = AL_FORMAT_MONO16; break;
            case 2: audioData->PCMFormat = AL_FORMAT_STEREO16; break;
            default: LOG_ERROR("Unsupported Audio Channel count %i!", wav.channels); break;
        }

        drwav_uninit(&wav);
        free(data);

        return true;
    }
}  // namespace lr::AudioLoader