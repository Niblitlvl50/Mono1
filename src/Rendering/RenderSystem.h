
#pragma once

#include <stddef.h>

namespace mono
{
    struct RenderInitParams
    {
        float pixels_per_meter = 32.0f;
        size_t n_sprites = 100;
    };

    void InitializeRender(const RenderInitParams& init_params);
    void ShutdownRender();

    void LoadCustomShaderFactory(const class IShaderFactory* custom_factory);
    const class SpriteInstanceCollection* GetSpriteInstances();
}
