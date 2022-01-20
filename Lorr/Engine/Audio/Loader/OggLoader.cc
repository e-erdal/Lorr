#include "OggLoader.hh"

#include <AL/al.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace lr::AudioLoader
{
    // Vorbis masscare

    typedef struct
    {
        unsigned char *data;
        ogg_int64_t size;
        ogg_int64_t pos;

    } VorbisMem;

    static size_t OAL_OggBufferRead(void *dest, size_t eltSize, size_t nelts, VorbisMem *src)
    {
        size_t len = eltSize * nelts;
        if ((src->pos + len) > src->size) len = src->size - src->pos;
        if (len > 0)
        {
            memcpy(dest, (src->data + src->pos), len);
            src->pos += len;
        }

        return len;
    }

    static int OAL_OggBufferSeek(VorbisMem *src, ogg_int64_t pos, int whence)
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

        if (src->pos > src->size) return -1;

        return 0;
    }

    static int OAL_OggBufferClose(VorbisMem *src)
    {
        return 0;
    }

    static long OAL_OggBufferTell(VorbisMem *src)
    {
        return src->pos;
    }

    static ov_callbacks OAL_CALLBACKS_BUFFER = {
        (size_t(*)(void *, size_t, size_t, void *))OAL_OggBufferRead,
        (int (*)(void *, ogg_int64_t, int))OAL_OggBufferSeek,
        (int (*)(void *))OAL_OggBufferClose,
        (long (*)(void *))OAL_OggBufferTell,
    };

    bool OggAudioLoader::LoadBinary(AudioData *audioData, BufferStream &buf)
    {
        VorbisMem vm = { buf.GetData(), (ogg_int64_t)buf.GetSize(), 0 };
        OggVorbis_File vf;

        if (ov_open_callbacks(&vm, &vf, NULL, 0, OAL_CALLBACKS_BUFFER) < 0)
        {
            LOG_ERROR("Failed to load ogg.");
            return false;
        }

        vorbis_info *vi = ov_info(&vf, -1);
        long sampleRate = vi->rate;
        u32 channels = vi->channels;
        u64 samples = ov_pcm_total(&vf, -1);

        u32 dataLen = sizeof(i16) * channels * samples;
        u8 *pcmBuf = (u8 *)malloc(dataLen);

        bool finished = false;
        uintptr_t offset = 0;
        while (!finished)
        {
            int cur = 0;
            long len = ov_read(&vf, (char *)pcmBuf + offset, 4096, 0, 2, 1, &cur);
            offset += len;

            if (len == 0)
                finished = true;
            else if (len < 0 && len == OV_EBADLINK)
                LOG_ERROR("Bitstream error @ %llu.", offset);
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
}  // namespace lr::AudioLoader