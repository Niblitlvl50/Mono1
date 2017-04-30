
#pragma once

#include "Math/Vector.h"
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
