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
        
        virtual void AddEntity(const IEntityPtr& entity, int layer);
        virtual void RemoveEntity(const IEntityPtr& entity);
        
        virtual void AddUpdatable(const IUpdatablePtr& updatable);
        virtual void RemoveUpdatable(const IUpdatablePtr& updatable);
        
        virtual void AddDrawable(const IDrawablePtr& drawable, int layer);
        virtual void RemoveDrawable(const IDrawablePtr& drawable);
        
        virtual void ClearEntities();

    private:
        
        std::map<int, std::vector<IDrawablePtr>> mLayersDrawables;
        std::map<int, std::vector<IEntityPtr>> mLayersEntities;
        std::vector<IUpdatablePtr> mUpdatables;
    };
    
}
