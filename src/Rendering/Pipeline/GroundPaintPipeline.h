
#pragma once

#include "Math/MathFwd.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Pipeline/IPipeline.h"

namespace mono
{
    struct GroundPaintDrawParams;

    // Draws the ground texture layer that wins at each pixel, picked from the splat map weights with
    // some noise so the edges between layers are ragged but crisp.
    class GroundPaintPipeline
    {
    public:

        static mono::IPipelinePtr MakePipeline();

        static void Apply(
            IPipeline* pipeline,
            const IRenderBuffer* position,
            const IElementBuffer* indices,
            const GroundPaintDrawParams& params);

        static void SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model);
        static void SetParams(const GroundPaintDrawParams& params, float pixels_per_meter);
    };
}
