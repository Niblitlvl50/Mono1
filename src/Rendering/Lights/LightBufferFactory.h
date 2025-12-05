
#pragma once

#include "Math/MathFwd.h"
#include "Rendering/RenderFwd.h"
#include <memory>
#include <vector>

namespace mono
{
    struct LightDrawBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> uv;
        std::unique_ptr<IRenderBuffer> colors;
        std::unique_ptr<IElementBuffer> indices;
    };

    LightDrawBuffers AllocateLightDrawBuffers(uint32_t num_lights);

    void UpdateLightDrawBuffer(
        const std::vector<math::Vector>& vertex_data,
        const std::vector<math::Vector>& uv_data,
        const std::vector<mono::Color::RGBA>& color_data,
        const std::vector<uint16_t>& index_data,
        LightDrawBuffers& draw_buffers);
}
