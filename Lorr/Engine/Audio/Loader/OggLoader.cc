#include "OggLoader.hh"

#include <AL/al.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace Lorr::AudioLoader
{
    // Vorbis masscare

    typedef struct
    {
        unsigned char *data;
        ogg_int64_t size;
        ogg_int64_t pos;

    } OAL_OggMemoryFile;

    static size_t OAL_OggBufferRead(void *dest, size_t eltSize, size_t nelts, OAL_OggMemoryFile *src)
    {
        size_t len = eltSize * nelts;

        if ((src->pos + len) > src->size)
        {
            len = src->size - src->pos;
        }

        if (len > 0)
        {
            memcpy(dest, (src->data + src->pos), len);
            src->pos += len;
        }

        return len;
    }

    static int OAL_OggBufferSeek(OAL_OggMemoryFile *src, ogg_int64_t pos, int whence)
    {
        switch (whence)
        {
            case SEEK_CUR: src->pos += pos; break;
            case SEEK_END: src->pos = src->size - pos; break;
            case SEEK_SET: src->pos = pos; break;

            default: return -1;
        }

        if (src->pos < 0)
        {
            src->pos = 0;
            return -1;
        }

        if (src->pos > src->size)
        {
            return -1;
        }

        return 0;
    }

    static int OAL_OggBufferClose(OAL_OggMemoryFile *src)
    {
        return 0;
    }

    static long OAL_OggBufferTell(OAL_OggMemoryFile *src)
    {
        return src->pos;
    }

    static ov_callbacks OAL_CALLBACKS_BUFFER = {

        (size_t(*)(void *, size_t, size_t, void *))OAL_OggBufferRead, (int (*)(void *, ogg_int64_t, int))OAL_OggBufferSeek, (int (*)(void *))OAL_OggBufferClose,
        (long (*)(void *))OAL_OggBufferTell

    };

    bool OggAudioLoader::LoadBinary(AudioData *audioData, BufferStream &buf)
    {
        OAL_OggMemoryFile fakeFile = { buf.GetData(), (ogg_int64_t)buf.GetSize(), 0 };
        OggVorbis_File vf;
        
        if (ov_open_callbacks(&fakeFile, &vf, NULL, 0, OAL_CALLBACKS_BUFFER) < 0)
        {
            LOG_ERROR("Failed to load ogg.");
            return false;
        }

        vorbis_info *vi = ov_info(&vf, -1);
        auto sampleRate = vi->rate;
        uint32_t channels = vi->channels;
        uint64_t samples = ov_pcm_total(&vf, -1);

        uint32_t dataLen = sizeof(int16_t) * channels * samples;
        uint8_t *pcmBuf = (uint8_t *)malloc(dataLen);

        for (size_t size = 0, offset = 0, sel = 0; (size = ov_read(&vf, (char *)pcmBuf + offset, 4096, 0, 2, 1, (int *)&sel)) != 0; offset += size)
        {
        }

        ov_clear(&vf);

        audioData->PCMFrequency = sampleRate;
        audioData->PCMFrames.Reset(pcmBuf, dataLen);

        switch (channels)
        {
            case 1: audioData->PCMFormat = AL_FORMAT_MONO16; break;
            case 2: audioData->PCMFormat = AL_FORMAT_STEREO16; break;
            default: CHECK(false, "Unsupported Audio Channel count %i!", channels); break;
        }

        free(pcmBuf);

        return true;
    }
}  // namespace Lorr::AudioLoader