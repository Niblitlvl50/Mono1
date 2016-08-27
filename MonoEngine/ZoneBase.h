//
//  ZoneBase.h
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IZone.h"
#include <vector>

namespace mono
{

    class ZoneBase : public virtual IZone
    {

    public:

        virtual void AddEntity(const IEntityPtr& entity, int layer);
        virtual void RemoveEntity(const IEntityPtr& entity);
        
        virtual void AddUpdatable(const IUpdatablePtr& updatable);
        virtual void RemoveUpdatable(const IUpdatablePtr& updatable);
        
        virtual void AddDrawable(const IDrawablePtr& drawable, int layer);
        virtual void RemoveDrawable(const IDrawablePtr& drawable);

        virtual mono::IEntityPtr FindEntityFromId(uint id) const;
        virtual void SchedulePreFrameTask(const std::function<void ()>& task);

    protected:

        virtual void Accept(IRenderer& renderer);
        virtual void DoPreAccept();

    private:
        
        std::vector<IEntityPtr> mEntities;
        std::vector<IUpdatablePtr> mUpdatables;
        std::vector<std::pair<int, IDrawablePtr>> mDrawables;

        std::vector<std::function<void ()>> m_preFrameTasks;
    };
    
}
