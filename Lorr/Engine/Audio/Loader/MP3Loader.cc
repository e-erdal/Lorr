#include "MP3Loader.hh"

#include <AL/al.h>
#include <dr_mp3.h>

namespace lr::AudioLoader
{
    bool MP3AudioLoader::LoadBinary(AudioData *audioData, BufferStream &buf)
    {
        drmp3 mp3;
        if (!drmp3_init_file(&mp3, "theme.mp3", nullptr))
        {
            LOG_WARN("Failed to load MP3 file.");
            return false;
        }

        u64 frameCount = drmp3_get_pcm_frame_count(&mp3);
        size_t dataLen = frameCount * mp3.channels * sizeof(u16);
        drmp3_int16 *data = (drmp3_int16 *)malloc(dataLen);

        drmp3_read_pcm_frames_s16(&mp3, frameCount, data);

        audioData->PCMFrames.Reset((u8 *)data, dataLen);
        audioData->PCMFrequency = mp3.mp3FrameSampleRate;

        switch (mp3.channels)
        {
            case 1: audioData->PCMFormat = AL_FORMAT_MONO16; break;
            case 2: audioData->PCMFormat = AL_FORMAT_STEREO16; break;
            default: LOG_ERROR("Unsupported Audio Channel count %i!", mp3.channels); break;
        }

        drmp3_uninit(&mp3);
        free(data);

        return true;
    }
}  // namespace lr::AudioLoader