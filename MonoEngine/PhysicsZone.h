
#pragma once

#include "IPhysicsZone.h"
#include "ZoneBase.h"
#include "Math/MathFwd.h"
#include "Physics/CMFwd.h"

#include <vector>

namespace mono
{
    class PhysicsZone : public ZoneBase, public IPhysicsZone
    {
    protected:
        
        PhysicsZone(const math::Vector& gravity, float damping);

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
