/*
 *  IDrawable.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MonoFwd.h"
#include "Math/MathFwd.h"

namespace mono
{
    struct IDrawable
    {
        virtual ~IDrawable()
        { }
		
        virtual void doDraw(mono::IRenderer& renderer) const = 0;

        // Bounding box in global coordiantes
        virtual math::Quad BoundingBox() const = 0;
    };
}


