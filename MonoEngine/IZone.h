//
//  IZone.h
//  Monolith1
//
//  Created by Niblit on 2012-03-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoPtrFwd.h"

namespace mono
{    
    enum LayerId
    {
        BACKGROUND,
        MIDDLEGROUND,
        FOREGROUND
    };

    struct IZone
    {
        virtual ~IZone()
        { }
        
        virtual void Accept(mono::IRenderer& renderer) = 0;
        virtual void OnLoad(mono::ICameraPtr camera) = 0;
        virtual void OnUnload() = 0;
                
        virtual void AddEntity(const mono::IEntityPtr& entity, int layer) = 0;
        virtual void RemoveEntity(const mono::IEntityPtr& entity) = 0;
        
        virtual void AddDrawable(const mono::IDrawablePtr& drawable, int layer) = 0;
        virtual void RemoveDrawable(const mono::IDrawablePtr& drawable) = 0;

        virtual void AddUpdatable(const mono::IUpdatablePtr& updatable) = 0;
        virtual void RemoveUpdatable(const mono::IUpdatablePtr& updatable) = 0;
    };
}

