
#pragma once

#include "RenderFwd.h"
#include "BlendMode.h"
#include "Math/MathFwd.h"
#include "Shader/ShaderFwd.h"
#include <vector>
#include <stdlib.h>

namespace mono
{
    class IRenderBuffer;

    //! Draws a quad
    void DrawQuad(const math::Quad& quad, const mono::Color::RGBA& color, float width, IColorShader* shader);

    void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color, IColorShader* shader);

    //! Draws a circle
    void DrawCircle(const math::Vector& position,
                    float radie,
                    unsigned int segments,
                    float lineWidth,
                    const mono::Color::RGBA& color,
                    IColorShader* shader);

    //! Draws a sprite
    void DrawSprite(
        const math::Quad& sprite_coords, const math::Vector& size, const math::Vector& offset, float pixels_per_meter, ITextureShader* shader);

    //! Draws texts from the text definitions
    void DrawText(const struct TextDefinition& text, ITextureShader* shader);

    //! Draws a single line
    void DrawLine(
        const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IColorShader* shader);

    void DrawClosedLine(
        const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IColorShader* shader);

    //! Draws lines, not connected to each other
    void DrawLines(
        const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IColorShader* shader);

    // Draw a collection of points
    void DrawPoints(
        const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float size, IColorShader* shader);

    void DrawTexturedGeometry(const std::vector<math::Vector>& vertices,
                              const std::vector<math::Vector>& texture_coordinates,
                              const std::vector<unsigned short>& indices,
                              ITextureShader* shader);

    void DrawTexturedGeometry(const mono::IRenderBuffer* vertices,
                              const mono::IRenderBuffer* texture_coordinates,
                              size_t offset,
                              size_t count,
                              ITextureShader* shader);

    void DrawTrianges(
        const mono::IRenderBuffer* vertices,
        const mono::IRenderBuffer* colors,
        const mono::IRenderBuffer* indices,
        size_t count,
        IColorShader* shader);

    void DrawParticlePoints(const mono::IRenderBuffer* position,
                            const mono::IRenderBuffer* color,
                            const mono::IRenderBuffer* point_size,
                            size_t count,
                            BlendMode blend_mode,
                            IPointSpriteShader* shader);

    void DrawPolyline(
        const mono::IRenderBuffer* vertices,
        const mono::IRenderBuffer* colors,
        size_t offset,
        size_t count,
        float width,
        IColorShader* shader);
}
