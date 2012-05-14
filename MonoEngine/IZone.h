//
//  IZone.h
//  Monolith1
//
//  Created by Niblit on 2012-03-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"

namespace mono
{
    
    struct IZone
    {
        virtual ~IZone()
        { }
        
        virtual void Accept(IRenderer& renderer) = 0;
        virtual void OnLoad(ICameraPtr camera) = 0;
        virtual void OnUnload() = 0;        
    };

}

