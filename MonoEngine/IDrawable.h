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

namespace mono
{

    struct IDrawable
    {
        virtual ~IDrawable()
        { }
		
        virtual void doDraw(IRenderer& renderer) const = 0;
    };

}


