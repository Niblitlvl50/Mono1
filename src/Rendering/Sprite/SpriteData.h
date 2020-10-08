
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
        struct Frame
        {
            int frame;
            int duration;
        };

        std::string name;
        std::vector<Frame> frames;
        bool looping;
        int frame_rate;
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
