
#pragma once

#include "Math/MathFwd.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Pipeline/IPipeline.h"

namespace mono
{
    class ColorPipeline
    {
    public:

        static mono::IPipelinePtr MakePointsPipeline();
        static mono::IPipelinePtr MakeLinesPipeline();
        static mono::IPipelinePtr MakeLineStripPipeline();
        static mono::IPipelinePtr MakeTrianglesPipeline();

        static void Apply(IPipeline* pipeline, const IRenderBuffer* position, const IRenderBuffer* color);
        static void Apply(
            IPipeline* pipeline, const IRenderBuffer* vertices, const IRenderBuffer* colors, const IElementBuffer* indices);

        static void SetTime(float total_time_s, float delta_time_s);
        static void SetTransforms(const math::Matrix& projection, const math::Matrix& view, const math::Matrix& model);
        static void SetPointSize(float point_size);
        static void SetLineWidth(float line_width);
    };
}
