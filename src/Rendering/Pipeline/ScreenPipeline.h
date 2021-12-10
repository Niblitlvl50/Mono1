
#pragma once

#include "Rendering/RenderFwd.h"
#include "Rendering/Pipeline/IPipeline.h"

namespace mono
{
    class ScreenPipeline
    {
    public:

        static mono::IPipelinePtr MakePipeline();
        static void Apply(
            IPipeline* pipeline,
            const IRenderBuffer* position,
            const IRenderBuffer* uv_coordinates,
            const IElementBuffer* indices,
            const ITexture* color_texture,
            const ITexture* light_texture);

        static void FadeCorners(bool enable);
        static void InvertColors(bool enable);
        static void FadeScreenAlpha(float alpha);
    };
}
