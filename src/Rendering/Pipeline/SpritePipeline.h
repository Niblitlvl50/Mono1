
#pragma once

#include "Math/MathFwd.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Pipeline/IPipeline.h"

namespace mono
{
    class SpritePipeline
    {
    public:

        static mono::IPipelinePtr MakePipeline();
        static void Apply(
            IPipeline* pipeline,
            const IRenderBuffer* position,
            const IRenderBuffer* uv_coordinates,
            const IRenderBuffer* heights,
            const IElementBuffer* indices,
            const ITexture* texture);

        static void SetTime(float total_time_s, float delta_time_s);
        static void SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model);

        static void SetWindSway(bool enable_wind);
        static void SetShade(const mono::Color::RGBA& color);
        static void SetFlashSprite(bool flash);
    };
}