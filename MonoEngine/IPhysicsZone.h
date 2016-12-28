
#pragma once

#include "IZone.h"
#include "MonoPtrFwd.h"
#include "Physics/CMFwd.h"

namespace mono
{
    struct IPhysicsZone : public virtual IZone
    {
        virtual void ForEachBody(const mono::BodyFunc& func) = 0;
        virtual IPhysicsEntityPtr FindPhysicsEntityFromBody(const mono::IBodyPtr& body) const = 0;
        virtual mono::IPhysicsEntityPtr FindPhysicsEntityFromId(uint id) const = 0;

        virtual void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer) = 0;
        virtual void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity) = 0;

        virtual void AddConstraint(const mono::IConstraintPtr& constraint) = 0;
        virtual void RemoveConstraint(const mono::IConstraintPtr& constraint) = 0;
    };
}
