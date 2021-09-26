#include "WavLoader.hh"

#include <AL/al.h>
#include <dr_wav.h>

namespace Lorr::AudioLoader
{
    bool WavAudioLoader::LoadBinary(AudioData *audioData, BufferStream &buf)
    {
        drwav wav;
        CHECK(drwav_init_memory(&wav, buf.GetData(), buf.GetSize(), NULL), "Failed to load WAV File!");

        std::vector<uint8_t> frames;
        frames.resize(wav.totalPCMFrameCount * wav.channels * wav.bitsPerSample);

        size_t numberOfSamplesActuallyDecoded = drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, (void *)frames.data());

        audioData->PCMFrames.Reset(frames);
        audioData->PCMFrequency = wav.sampleRate;

        switch (wav.channels)
        {
            case 1: audioData->PCMFormat = AL_FORMAT_MONO16; break;
            case 2: audioData->PCMFormat = AL_FORMAT_STEREO16; break;
            default: CHECK(false, "Unsupported Audio Channel count %i!", wav.channels); break;
        }

        drwav_uninit(&wav);

        return true;
    }
}  // namespace Lorr::AudioLoader