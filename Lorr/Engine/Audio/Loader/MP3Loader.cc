#include "MP3Loader.hh"

#include <AL/al.h>

#include <minimp3.h>
#include <minimp3_ex.h>

static mp3dec_t s_pMP3Ctx;
static bool init = false;

namespace Lorr::AudioLoader
{
    bool MP3AudioLoader::LoadBinary(AudioData *audioData, BufferStream &buf)
    {
        if (!init)
        {
            LOG_TRACE("MP3 init.");
            mp3dec_init(&s_pMP3Ctx);

            init = true;
        }

        mp3dec_file_info_t info;
        if (mp3dec_load(&s_pMP3Ctx, "theme.mp3", &info, NULL, NULL))
        {
            return false;
        }

        audioData->PCMFrames.Reset(info.samples * sizeof(mp3d_sample_t));
        audioData->PCMFrames.AssignPtr<mp3d_sample_t>(info.buffer, info.samples);
        audioData->PCMFrequency = info.hz;

        switch (info.channels)
        {
            case 1: audioData->PCMFormat = AL_FORMAT_MONO16; break;
            case 2: audioData->PCMFormat = AL_FORMAT_STEREO16; break;
            default: CHECK(false, "Unsupported Audio Channel count %i!", info.channels); break;
        }

        return true;
    }
}  // namespace Lorr::AudioLoader