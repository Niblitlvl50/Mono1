//
//  AudioFactory.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/01/16.
//
//

#pragma once

#include "ISound.h"
#include "SysTypes.h"
#include <string>
#include <vector>

namespace mono
{
    struct SoundFile
    {
        int frequency;
        int format;
        std::vector<byte> data;
    };

    namespace AudioFactory
    {
        mono::ISoundPtr CreateSound(const std::string& file);

        SoundFile LoadFile(const std::string& file);
    }
};