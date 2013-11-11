//
//  RenderUtils.h
//  Mono1
//
//  Created by Niblit on 2013-03-17.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MathFwd.h"

namespace mono
{
    class Sprite;
    class Color;
    
    //! Draws a quad
    void DrawQuad(const math::Quad& quad, const mono::Color& color);
    
    //! Draws a circle
    void DrawCircle(const math::Vector2f& position, float radie, int segments, const mono::Color& color);
    
    //! Draws a sprite
    void DrawSprite(const Sprite& sprite);
}

