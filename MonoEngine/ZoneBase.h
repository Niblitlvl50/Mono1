//
//  ZoneBase.h
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IZone.h"
#include <map>
#include <vector>

namespace mono
{

    class ZoneBase : public IZone
    {
    protected:
        
        ZoneBase();
        
        
        virtual void Accept(IRenderer& renderer);
        virtual void DoPreAccept();
        
        virtual void AddEntity(IEntityPtr entity, int layer);
        virtual void RemoveEntity(IEntityPtr entity);
        
        virtual void AddUpdatable(IUpdatablePtr updatable);
        virtual void RemoveUpdatable(IUpdatablePtr updatable);
        
        virtual void AddDrawable(IDrawablePtr drawable, int layer);
        virtual void RemoveDrawable(IDrawablePtr drawable);
        
        virtual void ClearEntities();

    private:
        
        std::map<int, std::vector<IDrawablePtr>> mLayersDrawables;
        std::map<int, std::vector<IEntityPtr>> mLayersEntities;
        std::vector<IUpdatablePtr> mUpdatables;
    };
    
}
