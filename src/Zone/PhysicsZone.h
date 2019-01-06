
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
    public:

        const std::vector<IPhysicsEntityPtr>& GetPhysicsEntities() const;

    protected:
        
        PhysicsZone(const math::Vector& gravity, float damping);

        void ForEachBody(const mono::BodyFunc& func) override;
        IPhysicsEntityPtr FindPhysicsEntityFromBody(const mono::IBody* body) const override;
        IPhysicsEntityPtr FindPhysicsEntityFromId(unsigned int id) const override;

        void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer) override;
        void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity) override;

        void AddConstraint(const mono::IConstraintPtr& constraint) override;
        void RemoveConstraint(const mono::IConstraintPtr& constraint) override;

        void AddPhysicsData(const mono::PhysicsData& physics_data) override;
        void RemovePhysicsData(const mono::PhysicsData& physics_data) override;

        void DoPreAccept() override;

        struct PhysicsImpl;
        std::shared_ptr<PhysicsImpl> m_physics;
        std::vector<IPhysicsEntityPtr> m_physics_entities;
        std::vector<IConstraintPtr> m_constraints;

        std::vector<IPhysicsEntityPtr> m_physics_remove_list;
    };
}
