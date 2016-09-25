//
//  AudioFactory.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/01/16.
//
//

#pragma once

#include "ISound.h"
#include "System/SysTypes.h"
#include "MonoPtrFwd.h"

#include <vector>

namespace mono
{
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
        std::vector<byte> data;
    };

    namespace AudioFactory
    {
        // Create a sound to use, only supports WAV at the moment
        mono::ISoundPtr CreateSound(const char* fileName, bool loop, bool relative);

        // Load a sound file from disk, only supports WAV at the moment
        SoundFile LoadFile(const char* fileName);
    }
};