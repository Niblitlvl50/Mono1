
#pragma once

#include "IZone.h"
#include "MonoPtrFwd.h"
#include "Physics/CMFwd.h"

namespace mono
{
    using BodyFunc = std::function<void (const IBodyPtr&)>;

    class IPhysicsZone : public virtual mono::IZone
    {
    public:

        virtual void ForEachBody(const mono::BodyFunc& func) = 0;
        virtual IPhysicsEntityPtr FindPhysicsEntityFromBody(const mono::IBodyPtr& body) const = 0;
        virtual mono::IPhysicsEntityPtr FindPhysicsEntityFromId(unsigned int id) const = 0;

        virtual void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer) = 0;
        virtual void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity) = 0;

        virtual void AddConstraint(const mono::IConstraintPtr& constraint) = 0;
        virtual void RemoveConstraint(const mono::IConstraintPtr& constraint) = 0;

        virtual void AddPhysicsData(const mono::PhysicsData& physics_data) = 0;
        virtual void RemovePhysicsData(const mono::PhysicsData& physics_data) = 0;
    };
}
