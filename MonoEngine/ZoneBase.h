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

    struct ZoneBase : public IZone
    {
    
    protected:
        
        ZoneBase();
        
        enum LayerId
        {
            BACKGROUND,
            MIDDLEGROUND,
            FOREGROUND
        };
        
        virtual void Accept(IRenderer& renderer);
        virtual void DoPreAccept();
        
        virtual void ClearEntities();

        void AddEntityToLayer(IEntityPtr entity, LayerId layer);
        void RemoveEntity(IEntityPtr entity);
        
        void AddUpdatable(IUpdatablePtr updatable);
        void RemoveUpdatable(IUpdatablePtr updatable);
        
    private:
        
        std::map<LayerId, std::vector<IEntityPtr>> mLayers;
        std::vector<IUpdatablePtr> mUpdatables;
    };
    
}
