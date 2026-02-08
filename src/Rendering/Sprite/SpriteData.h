
#pragma once

#include "Math/Vector.h"

#include <string>
#include <vector>
#include <cstdint>

namespace mono
{
    struct SpriteFrame
    {
        math::Vector uv_upper_left;
        math::Vector uv_lower_right;
        math::Vector center_offset;
        math::Vector size;
    };

    struct SpriteAnimationFrame
    {
        int frame;
        std::string notify;
    };

    struct SpriteAnimation
    {
        std::string name;
        std::vector<SpriteAnimationFrame> frames;
        bool looping;
        int frame_duration;
    };

    struct SpriteData
    {
        uint32_t hash;
        std::string texture_file;
        std::string source_folder;
        math::Vector texture_size;
        std::vector<SpriteFrame> frames;
        std::vector<SpriteAnimation> animations;
    };
}
