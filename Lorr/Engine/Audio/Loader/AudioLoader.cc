#include "AudioLoader.hh"

#include "WavLoader.hh"
#include "OggLoader.hh"
#include "MP3Loader.hh"

namespace Lorr::AudioLoader
{
    bool LoadAudio(AudioLoaderType eType, AudioData *audioData, BufferStream &buf)
    {
        switch (eType)
        {
            case AudioLoaderType::WAV:
            {
                WavAudioLoader loader;
                return loader.LoadBinary(audioData, buf);
            }
            case AudioLoaderType::OGG:
            {
                OggAudioLoader loader;
                return loader.LoadBinary(audioData, buf);
            }
            case AudioLoaderType::MP3:
            {
                MP3AudioLoader loader;
                return loader.LoadBinary(audioData, buf);
            }
            default:
            {
                LOG_ERROR("Unimplemented Audio File Format!");
            }
            break;
        }

        return false;
    }
}  // namespace Lorr::AudioLoader