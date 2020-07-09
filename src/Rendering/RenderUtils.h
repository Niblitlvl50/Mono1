
#pragma once

#include "RenderFwd.h"
#include "BlendMode.h"
#include "Math/MathFwd.h"
#include <vector>
#include <stdlib.h>
#include <cstdint>

namespace mono
{
    class IRenderBuffer;
    class IShader;

    //! Draws a quad
    void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width, IShader* shader);

    void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color, IShader* shader);

    //! Draws a circle
    void DrawCircle(const math::Vector& position,
                    float radie,
                    uint32_t segments,
                    float lineWidth,
                    const mono::Color::RGBA& color,
                    IShader* shader);

    //! Draws a sprite
    void DrawSprite(
        const math::Quad& sprite_coords, const math::Vector& size, const math::Vector& offset, IShader* shader);

    void DrawScreen(const math::Quad& sprite_coords, const math::Vector& size, IShader* shader);

    //! Draws texts from the text definitions
    void DrawText(const struct TextDefinition& text, IShader* shader);

    //! Draws a single line
    void DrawLine(
        const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IShader* shader);

    void DrawClosedLine(
        const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IShader* shader);

    //! Draws lines, not connected to each other
    void DrawLines(
        const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IShader* shader);

    // Draw a collection of points
    void DrawPoints(
        const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float size, IShader* shader);

    void DrawTexturedGeometry(const std::vector<math::Vector>& vertices,
                              const std::vector<math::Vector>& texture_coordinates,
                              const std::vector<uint16_t>& indices,
                              IShader* shader);

    void DrawTexturedGeometry(const mono::IRenderBuffer* vertices,
                              const mono::IRenderBuffer* texture_coordinates,
                              size_t offset,
                              size_t count,
                              IShader* shader);

    void DrawTrianges(
        const mono::IRenderBuffer* vertices,
        const mono::IRenderBuffer* colors,
        const mono::IRenderBuffer* indices,
        size_t count,
        IShader* shader);

    void DrawParticlePoints(
        const mono::IRenderBuffer* position,
        const mono::IRenderBuffer* rotation,
        const mono::IRenderBuffer* color,
        const mono::IRenderBuffer* point_size,
        size_t count,
        BlendMode blend_mode,
        IShader* shader);

    void DrawPolyline(
        const mono::IRenderBuffer* vertices,
        const mono::IRenderBuffer* colors,
        size_t offset,
        size_t count,
        float width,
        IShader* shader);
}
