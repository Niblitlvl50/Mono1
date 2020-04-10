
#pragma once

#include "ISound.h"
#include <vector>
#include <memory>

namespace mono
{
    using ISoundPtr = std::unique_ptr<ISound>;

    enum class SoundFormat
    {
        MONO_8BIT,
        MONO_16BIT,
        STEREO_8BIT,
        STEREO_16BIT
    };

    struct SoundFile
    {
        int frequency;
        SoundFormat format;
        std::vector<unsigned char> data;
    };

    enum class SoundPlayback
    {
        LOOPING,
        ONCE
    };

    enum class SoundPosition
    {
        RELATIVE,
        GLOBAL
    };

    namespace AudioFactory
    {
        // Create a sound to use, only supports WAV at the moment
        mono::ISoundPtr CreateSound(const char* file_name, SoundPlayback playback, SoundPosition position);
        mono::ISoundPtr CreateNullSound();

        // Load a sound file from disk, only supports WAV at the moment
        SoundFile LoadFile(const char* file_name);
    }
}
