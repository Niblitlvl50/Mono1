
#pragma once

#include <stddef.h>

namespace mono
{
    struct RenderInitParams
    {
        float pixels_per_meter = 32.0f;
        const char* imgui_ini = nullptr;
    };

    void InitializeRender(const RenderInitParams& init_params);
    void ShutdownRender();

    void LoadCustomTextureFactory(const class ITextureFactory* texture_factory);

    const class ISpriteFactory* GetSpriteFactory();
    const class ITextureFactory* GetTextureFactory();
}
