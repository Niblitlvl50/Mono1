
#pragma once

#include "PhysicsFwd.h"
#include "Math/MathFwd.h"

#include <functional>
#include <memory>

struct cpSpace;
struct cpArbiter;

namespace cm
{
    class BodyImpl;
}

namespace mono
{
    using QueryFilter = const std::function<bool (uint32_t entity_id, const math::Vector& point)>;

    struct QueryResult
    {
        mono::IBody* body;
        uint32_t collision_category;
    };

    class PhysicsSpace
    {
    public:
        
        PhysicsSpace(PhysicsSystem* physics_system);
        PhysicsSpace(PhysicsSystem* physics_system, const math::Vector& gravity, float damping);
        ~PhysicsSpace();

        void Tick(uint32_t delta);

        void SetGravity(const math::Vector& gravity);
        void SetDamping(float damping);
        
        void Add(IBody* body);
        void Add(IShape* shape);
        void Add(IConstraint* constraint);
        
        void Remove(IBody* body);
        void Remove(IShape* shape);
        void Remove(IConstraint* constraint);
        
        QueryResult QueryFirst(const math::Vector& start, const math::Vector& end, uint32_t category);
        QueryResult QueryNearest(const math::Vector& point, float max_distance, uint32_t category);
        QueryResult QueryNearest(const math::Vector& point, float max_distance, uint32_t category, const QueryFilter& filter_func);

        std::vector<QueryResult> QueryAllInLIne(const math::Vector& start, const math::Vector& end, float max_distance, uint32_t category);
        std::vector<QueryResult> QueryBox(const math::Quad& world_bb, uint32_t category);
        std::vector<QueryResult> QueryRadius(const math::Vector& position, float radius, uint32_t category);

        IBody* GetStaticBody();

        cpSpace* Handle();
        
    private:

        bool OnCollision(cpArbiter* arb);
        void OnSeparation(cpArbiter* arb);

        PhysicsSystem* m_physics_system;
        cpSpace* m_space;
        std::unique_ptr<cm::BodyImpl> m_static_body;
    };
}
