
#pragma once

#include "PhysicsFwd.h"
#include "Math/MathFwd.h"
#include <vector>
#include <functional>

struct cpSpace;
struct cpArbiter;

namespace mono
{
    using QueryFilter = const std::function<bool (uint32_t entity_id, const math::Vector& point)>;
    using BodyFunc = std::function<void (IBody*)>;

    class PhysicsSpace
    {
    public:
        
        PhysicsSpace();
        PhysicsSpace(const math::Vector& gravity, float damping);
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
        
        void ForEachBody(const BodyFunc& func);
        IBody* QueryFirst(const math::Vector& start, const math::Vector& end, uint32_t category);
        std::vector<IBody*> QueryAllInLIne(const math::Vector& start, const math::Vector& end, float max_distance, uint32_t category);
        IBody* QueryNearest(const math::Vector& point, float max_distance, uint32_t category);
        IBody* QueryNearest(
            const math::Vector& point, float max_distance, uint32_t category, const QueryFilter& filter_func);

        cpSpace* Handle();
        
    private:

        bool OnCollision(cpArbiter* arb);
        void DoForEachFuncOnBody(cpBody* body);

        cpSpace* m_space;
        BodyFunc m_for_each_func;
        std::vector<IBody*> m_bodies;
    };
}
