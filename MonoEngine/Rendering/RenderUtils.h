
#pragma once

#include "RenderFwd.h"
#include "Math/MathFwd.h"
#include "Shader/ShaderFwd.h"
#include <vector>
#include <stdlib.h>

namespace mono
{
    struct TextDefinition;
    class IRenderBuffer;

    //! Draws a quad
    void DrawQuad(
        const math::Quad& quad, const mono::Color::RGBA& color, float width, IColorShader* shader);

    void DrawFilledQuad(const math::Quad& quad, const mono::Color::RGBA& color, IColorShader* shader);

    //! Draws a circle
    void DrawCircle(const math::Vector& position,
                    float radie,
                    unsigned int segments,
                    float lineWidth,
                    const mono::Color::RGBA& color,
                    IColorShader* shader);

    //! Draws a sprite
    void DrawSprite(const ISprite& sprite, const math::Vector& offset, ITextureShader* shader);

    //! Draws texts from the text definitions
    void DrawText(const TextDefinition& text, ITextureShader* shader);

    //! Draws a single line
    void DrawLine(
        const std::vector<math::Vector>& vertices, const mono::Color::RGBA& color, float width, IColorShader* shader);

    void DrawClosedLine(const std::vector<math::Vector>& vertices,
                        const mono::Color::RGBA& color,
                        float width,
                        IColorShader* shader);

    //! Draws lines, not connected to each other
    void DrawLines(const std::vector<math::Vector>& vertices,
                   const mono::Color::RGBA& color,
                   float width,
                   IColorShader* shader);

    // Draw a collection of points
    void DrawPoints(const std::vector<math::Vector>& vertices,
                    const mono::Color::RGBA& color,
                    float size,
                    IColorShader* shader);

    void DrawShape(const std::vector<math::Vector>& shape1,
                   const std::vector<math::Vector>& shape2,
                   const mono::Color::RGBA& color,
                   IMorphingShader* shader);

    void DrawTexturedGeometry(const std::vector<math::Vector>& vertices,
                              const std::vector<math::Vector>& texture_coordinates,
                              const std::vector<unsigned short>& indices,
                              ITextureShader* shader);

    void DrawTexturedGeometry(const mono::IRenderBuffer* vertices,
                              const mono::IRenderBuffer* texture_coordinates,
                              size_t offset,
                              size_t count,
                              ITextureShader* shader);

    void DrawParticlePoints(const mono::IRenderBuffer* position,
                            const mono::IRenderBuffer* color,
                            const mono::IRenderBuffer* point_size,
                            size_t count,
                            IPointSpriteShader* shader);

    void DrawPolyline(
        const mono::IRenderBuffer* vertices,
        const mono::IRenderBuffer* colors,
        size_t offset,
        size_t count,
        float width,
        IColorShader* shader);
}
