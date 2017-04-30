
#pragma once

#include "Math/Vector.h"
#include "System/File.h"
#include <vector>

namespace world
{
    struct PolygonData
    {
        math::Vector position;
        math::Vector local_offset;
        float rotation = 0.0f;
        float texture_repeate = 1.0f;
        char texture[64] = { 0 };
        std::vector<math::Vector> vertices;
    };

    struct LevelFileHeader
    {
        int version = 0;
        std::vector<PolygonData> polygons;
    };

    bool WriteWorld(File::FilePtr& file, const LevelFileHeader& level);
    bool ReadWorld(File::FilePtr& file, LevelFileHeader& level);
}
