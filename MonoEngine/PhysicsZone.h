//
//  PhysicsZone.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "IPhysicsZone.h"
#include "ZoneBase.h"
#include "MathFwd.h"
#include "CMFwd.h"

#include <vector>

namespace mono
{
    class PhysicsZone : public ZoneBase, public IPhysicsZone
    {
    protected:
        
        PhysicsZone(const math::Vector2f& gravity, float damping);

        virtual void ForEachBody(const mono::BodyFunc& func);
        virtual IPhysicsEntityPtr FindPhysicsEntityFromBody(const mono::IBodyPtr& body) const;
        virtual IPhysicsEntityPtr FindPhysicsEntityFromId(uint id) const;
        virtual void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer);
        virtual void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity);

    private:

        struct PhysicsImpl;
        std::shared_ptr<PhysicsImpl> mPhysics;
        std::vector<IPhysicsEntityPtr> mPhysicsEntities;
    };
}

