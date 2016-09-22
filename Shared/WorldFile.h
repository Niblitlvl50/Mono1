//
//  WorldFile.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 21/07/16.
//
//

#pragma once

#include "Math/Vector2f.h"
#include "System/SysFile.h"
#include <vector>

namespace world
{
    struct PolygonData
    {
        math::Vector2f position;
        math::Vector2f local_offset;
        float rotation = 0.0f;
        float texture_repeate = 1.0f;
        char texture[32] = { 0 };
        std::vector<math::Vector2f> vertices;
    };

    struct LevelFileHeader
    {
        int version = 0;
        std::vector<PolygonData> polygons;
    };

    bool WriteWorld(File::FilePtr& file, const LevelFileHeader& level);
    bool ReadWorld(File::FilePtr& file, LevelFileHeader& level);
}
