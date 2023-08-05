
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
        static mono::IPipelinePtr MakeOutlinePipeline();

        static void Apply(
            IPipeline* pipeline,
            const IRenderBuffer* position,
            const IRenderBuffer* offsets,
            const IRenderBuffer* uv_coordinates,
            const IRenderBuffer* uv_coordinates_flipped,
            const IRenderBuffer* heights,
            const IElementBuffer* indices,
            const ITexture* texture,
            uint32_t buffer_offset);

        static void SetTime(float total_time_s, float delta_time_s);
        static void SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model);

        static void SetFlipSprite(bool flip_horizontal, bool flip_vertical);
        static void SetWindSway(bool enable_wind);
        static void SetShade(const mono::Color::RGBA& color);
        static void SetFlashSprite(bool flash);
        static void SetOutlineColor(const mono::Color::RGBA& outline_color);
    };
}
