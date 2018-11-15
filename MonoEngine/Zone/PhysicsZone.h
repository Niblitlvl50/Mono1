
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
        virtual IPhysicsEntityPtr FindPhysicsEntityFromBody(const mono::IBody* body) const;
        virtual IPhysicsEntityPtr FindPhysicsEntityFromId(unsigned int id) const;

        virtual void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer);
        virtual void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity);

        virtual void AddConstraint(const mono::IConstraintPtr& constraint);
        virtual void RemoveConstraint(const mono::IConstraintPtr& constraint);

        virtual void AddPhysicsData(const mono::PhysicsData& physics_data);
        virtual void RemovePhysicsData(const mono::PhysicsData& physics_data);

    protected:

        struct PhysicsImpl;
        std::shared_ptr<PhysicsImpl> m_physics;
        std::vector<IPhysicsEntityPtr> m_physics_entities;
        std::vector<IConstraintPtr> m_constraints;
    };
}
