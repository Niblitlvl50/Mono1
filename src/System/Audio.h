
#pragma once

#include <memory>

namespace audio
{
    enum class SoundPlayback
    {
        LOOPING,
        ONCE
    };

    class ISound
    {
    public:

        virtual ~ISound() = default;
        virtual void Play() = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;
        virtual bool IsPlaying() const = 0;
    };

    using ISoundPtr = std::unique_ptr<ISound>;

    void Initialize();
    void Shutdown();

    audio::ISoundPtr CreateSound(const char* file_name, SoundPlayback playback);
    audio::ISoundPtr CreateNullSound();

    void MixSounds();
    void StopAllSounds();
    void ClearLoadedSounds();
}
