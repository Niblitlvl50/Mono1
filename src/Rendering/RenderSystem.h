
#pragma once

#include <stddef.h>

namespace System
{
    class IWindow;
}

namespace mono
{
    struct RenderInitParams
    {
        float pixels_per_meter = 32.0f;
        const char* imgui_ini = nullptr;
        System::IWindow* window = nullptr;
    };

    void InitializeRender(const RenderInitParams& init_params);
    void ShutdownRender();

    float PixelsPerMeter();
    float GetWindowAspect();

    void LoadCustomTextureFactory(const class ITextureFactory* texture_factory);

    const class ISpriteFactory* GetSpriteFactory();
    const class ITextureFactory* GetTextureFactory();
}
