
#pragma once

#include "Math/MathFwd.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Pipeline/IPipeline.h"
#include "Rendering/BlendMode.h"

namespace mono
{
    class ParticlePointPipeline
    {
    public:

        static mono::IPipelinePtr MakePipeline(mono::BlendMode blend_mode);
        static void Apply(
            IPipeline* pipeline,
            const IRenderBuffer* position,
            const IRenderBuffer* rotation,
            const IRenderBuffer* color,
            const IRenderBuffer* point_size,
            const ITexture* texture);

        //static void SetTime(float total_time_s, float delta_time_s);
        static void SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model);
    };
}
