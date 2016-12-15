
#pragma once

#include "MonoFwd.h"
#include "Math/MathFwd.h"
#include <vector>

namespace mono
{
    class IColorShader;
    class ITextureShader;
    class IMorphingShader;
    struct TextDefinition;

    //! Draws a quad
    void DrawQuad(const math::Quad& quad,
                  const mono::Color::RGBA& color,
                  float width,
                  const std::shared_ptr<IColorShader>& shader);

    //! Draws a circle
    void DrawCircle(const math::Vector& position,
                    float radie,
                    unsigned int segments,
                    float lineWidth,
                    const mono::Color::RGBA& color,
                    const std::shared_ptr<IColorShader>& shader);

    //! Draws a sprite
    void DrawSprite(const ISprite& sprite,
                    const std::shared_ptr<ITextureShader>& shader);

    //! Draws texts from the text definitions
    void DrawText(const TextDefinition& text,
                  const std::shared_ptr<ITextureShader>& shader);

    //! Draws a single line
    void DrawLine(const std::vector<math::Vector>& vertices,
                  const mono::Color::RGBA& color,
                  float width,
                  const std::shared_ptr<IColorShader>& shader);

    void DrawClosedLine(const std::vector<math::Vector>& vertices,
                        const mono::Color::RGBA& color,
                        float width,
                        const std::shared_ptr<IColorShader>& shader);

    //! Draws lines, not connected to each other
    void DrawLines(const std::vector<math::Vector>& vertices,
                   const mono::Color::RGBA& color,
                   float width,
                   const std::shared_ptr<IColorShader>& shader);

    // Draw a collection of points
    void DrawPoints(const std::vector<math::Vector>& vertices,
                    const mono::Color::RGBA& color,
                    float size,
                    const std::shared_ptr<IColorShader>& shader);

    void DrawShape(const std::vector<math::Vector>& shape1,
                   const std::vector<math::Vector>& shape2,
                   const mono::Color::RGBA& color,
                   const std::shared_ptr<IMorphingShader>& shader);

    void DrawTexturedGeometry(const std::vector<math::Vector>& vertices,
                              const std::vector<math::Vector>& texture_coordinates,
                              const std::vector<unsigned short>& indices,
                              const std::shared_ptr<ITextureShader>& shader);
}
