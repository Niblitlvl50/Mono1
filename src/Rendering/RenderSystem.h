
#pragma once

#include <stddef.h>

namespace mono
{
    struct RenderInitParams
    {
        float pixels_per_meter = 32.0f;
    };

    void InitializeRender(const RenderInitParams& init_params);
    void ShutdownRender();

    void LoadCustomShaderFactory(const class IShaderFactory* custom_factory);
}
