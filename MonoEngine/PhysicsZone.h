//
//  PhysicsZone.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "IPhysicsZone.h"
#include "MathFwd.h"
#include "CMFwd.h"

#include <map>
#include <vector>

namespace mono
{
    class PhysicsZone : public IPhysicsZone
    {
    protected:
        
        PhysicsZone(const math::Vector2f& gravity);
        virtual void Accept(IRenderer& renderer);
        
        virtual void ForEachBody(const cm::BodyFunc& func);

        virtual void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer);
        virtual void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity);
        
        virtual void AddEntity(const mono::IEntityPtr& entity, int layer);
        virtual void RemoveEntity(const mono::IEntityPtr& entity);
        
        virtual void AddDrawable(const mono::IDrawablePtr& drawable, int layer);
        virtual void RemoveDrawable(const mono::IDrawablePtr& drawable);
        
        virtual void AddUpdatable(const mono::IUpdatablePtr& updatable);
        virtual void RemoveUpdatable(const mono::IUpdatablePtr& updatable);
        
    private:
        
        void DoPreAccept();
        
        struct PhysicsImpl;
        std::shared_ptr<PhysicsImpl> mPhysics;
        
        std::vector<IPhysicsEntityPtr> mPhysicsEntities;
        std::vector<IEntityPtr>        mEntities;
        
        std::map<int, std::vector<IDrawablePtr>> mDrawables;
        std::vector<IUpdatablePtr>               mUpdatables;
    };
}

