//
//  EditorConfig.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 24/07/16.
//
//

#pragma once

#include "Math/Vector2f.h"
#include "Math/Quad.h"

namespace editor
{
    struct Config
    {
        math::Vector cameraPosition;
        math::Quad cameraViewport;
    };

    bool SaveConfig(const char* config_file, const Config& config);
    bool LoadConfig(const char* config_file, Config& config);
}
