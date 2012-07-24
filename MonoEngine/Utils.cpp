//
//  Utils.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Utils.h"
#include "IEntity.h"

namespace
{
    struct EntityFinder
    {
        EntityFinder(const mono::IEntityPtr entity)
            : mEntity(entity)
        { }
        bool operator()(const mono::IEntityPtr other) const
        {
            return mEntity == other;
        }
        const mono::IEntityPtr mEntity;
    };
}

bool mono::FindAndRemoveEntity(IEntityCollection& collection, IEntityPtr entity)
{
    const EntityFinder finder(entity);
    const IEntityCollection::iterator newEnd = std::remove_if(collection.begin(), collection.end(), finder);
    if(newEnd != collection.end())
    {
        collection.erase(newEnd, collection.end());
        return true;
    }
    
    return false;
}
