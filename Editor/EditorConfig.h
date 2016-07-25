//
//  EditorConfig.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 24/07/16.
//
//

#pragma once

#include "Vector2f.h"
#include "Quad.h"

namespace editor
{
    struct Config
    {
        math::Vector2f cameraPosition;
        math::Quad cameraViewport;
    };

    bool SaveConfig(const char* config_file, const Config& config);
    bool LoadConfig(const char* config_file, Config& config);
}
