
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
        static mono::IPipelinePtr MakeAnnotationPipeline();
        static mono::IPipelinePtr MakeVertexColorPipeline();

        static void Apply(
            IPipeline* pipeline,
            const IRenderBuffer* position,
            const IRenderBuffer* uv_coordinates,
            const IElementBuffer* indices,
            const ITexture* texture);

        static void Apply(
            IPipeline* pipeline,
            const IRenderBuffer* position,
            const IRenderBuffer* uv_coordinates,
            const IRenderBuffer* vertex_colors,
            const IElementBuffer* indices,
            const ITexture* texture);

        //static void SetTime(float total_time_s, float delta_time_s);
        static void SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model);

        static void SetIsAlpha(bool is_alpha_texture);
        static void SetBlur(bool enable_blur);
        static void SetShade(const mono::Color::RGBA& color);
    };
}
