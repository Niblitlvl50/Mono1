
#include "PrimitiveBufferFactory.h"
#include "Math/MathFunctions.h"
#include "Rendering/Color.h"
#include "Rendering/RenderBuffer/BufferFactory.h"

#include <vector>


mono::PrimitiveDrawBuffers mono::BuildCircleDrawBuffers(const math::Vector& size, int segments, const mono::Color::RGBA& color)
{
    if((segments % 2) != 0)
        segments += 1;

    std::vector<math::Vector> vertices;
    vertices.reserve(segments +1);
    vertices.push_back(math::ZeroVec);

    const uint16_t n_indices = segments * 3;
    std::vector<uint16_t> indices;
    indices.reserve(n_indices);

    const float coef = 2.0f * math::PI() / float(segments);

    for(int index = 0; index < segments; ++index)
    {
        const float radians = index * coef;
        const float x = size.x * std::cos(radians);
        const float y = size.y * std::sin(radians);
        vertices.emplace_back(x, y);

        indices.push_back(0);
        indices.push_back(index +1);
        indices.push_back(index +2);
    }

    indices.pop_back();
    indices.pop_back();

    indices.push_back(vertices.size() -1);
    indices.push_back(1);

    const std::vector<mono::Color::RGBA> colors(vertices.size(), color);

    PrimitiveDrawBuffers draw_buffers;
    draw_buffers.vertices = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, vertices.size(), vertices.data(), "prim_circle_buffer");
    draw_buffers.colors = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, colors.size(), colors.data(), "prim_circle_buffer");
    draw_buffers.indices = CreateElementBuffer(BufferType::STATIC, indices.size(), indices.data(), "prim_circle_buffer");

    return draw_buffers;
}