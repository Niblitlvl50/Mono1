
#pragma once

#include "Math/Vector.h"
#include "Math/Quad.h"

#include <string>
#include <vector>
#include <cstdint>

namespace mono
{
    struct SpriteFrame
    {
        math::Quad texture_coordinates;
        math::Vector center_offset;
        math::Vector size;
    };

    struct SpriteAnimation
    {
        std::string name;
        std::vector<int> frames;
        bool looping;
        int frame_duration;
    };

    struct SpriteData
    {
        uint32_t hash;
        std::string texture_file;
        std::string category;
        math::Vector texture_size;
        std::vector<SpriteFrame> frames;
        std::vector<SpriteAnimation> animations;
    };
}
