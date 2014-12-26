//
//  RenderUtils.h
//  Mono1
//
//  Created by Niblit on 2013-03-17.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MathFwd.h"
#include <vector>

namespace mono
{
    class Sprite;
    class Color;
    class ColorShader;
    class TextureShader;
    struct TextDefinition;
    
    //! Draws a quad
    void DrawQuad(const math::Quad& quad,
                  const mono::Color& color,
                  float width,
                  const std::shared_ptr<ColorShader>& shader);
    
    //! Draws a circle
    void DrawCircle(const math::Vector2f& position,
                    float radie,
                    int segments,
                    const mono::Color& color,
                    const std::shared_ptr<ColorShader>& shader);
    
    //! Draws a sprite
    void DrawSprite(const Sprite& sprite,
                    const std::shared_ptr<TextureShader>& shader);

    //! Draws texts from the text definitions
    void DrawTexts(const std::vector<TextDefinition>& texts,
                   const std::shared_ptr<TextureShader>& shader);

    //! Draws a single line
    void DrawLine(const std::vector<math::Vector2f>& vertices,
                  const mono::Color& color,
                  float width,
                  const std::shared_ptr<ColorShader>& shader);

    //! Draws lines, not connected to each other
    void DrawLines(const std::vector<math::Vector2f>& vertices,
                   const mono::Color& color,
                   float width,
                   const std::shared_ptr<ColorShader>& shader);

    // Draw a collection of points
    void DrawPoints(const std::vector<math::Vector2f>& vertices,
                    const mono::Color& color,
                    float size,
                    const std::shared_ptr<ColorShader>& shader);
}

