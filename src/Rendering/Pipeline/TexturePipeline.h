
#pragma once

#include "Math/MathFwd.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Pipeline/IPipeline.h"

namespace mono
{
    class TexturePipeline
    {
    public:

        static mono::IPipelinePtr MakePipeline();
        static void Apply(
            IPipeline* pipeline,
            const IRenderBuffer* position,
            const IRenderBuffer* uv_coordinates,
            const IElementBuffer* indices,
            const ITexture* texture);

        //static void SetTime(IPipeline* pipeline, float total_time_s, float delta_time_s);
        static void SetTransforms(
            IPipeline* pipeline, const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model);

        static void SetIsAlpha(IPipeline* pipeline, bool is_alpha_texture);
        static void SetShade(IPipeline* pipeline, const mono::Color::RGBA& color);
    };
}
