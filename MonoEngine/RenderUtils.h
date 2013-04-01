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
    
    // Draws a quad with the current color. 
    void DrawQuad(const math::Quad& quad);
    
    //! Draws a sprite
    void DrawSprite(const Sprite& sprite);
}

