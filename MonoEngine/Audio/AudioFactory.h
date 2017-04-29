
#pragma once

#include "ISound.h"
#include <vector>
#include <memory>

namespace mono
{
    using ISoundPtr = std::shared_ptr<ISound>;

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

    namespace AudioFactory
    {
        // Create a sound to use, only supports WAV at the moment
        mono::ISoundPtr CreateSound(const char* fileName, bool loop, bool relative);

        // Load a sound file from disk, only supports WAV at the moment
        SoundFile LoadFile(const char* fileName);
    }
};
