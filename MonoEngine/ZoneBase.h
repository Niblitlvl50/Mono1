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
        
        void AddEntityToLayer(IEntityPtr entity, LayerId layer);
        void RemoveEntity(IEntityPtr entity);
        
        void AddUpdatable(IUpdatablePtr updatable);
        void RemoveUpdatable(IUpdatablePtr updatable);
        
    private:
        
        typedef std::map<LayerId, IEntityCollection> LayerMap;
        LayerMap mLayers;
        IUpdatableCollection mUpdatables;
    };
    
}
