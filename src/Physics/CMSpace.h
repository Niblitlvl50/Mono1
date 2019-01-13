
#pragma once

#include "CMFwd.h"
#include "Math/MathFwd.h"
#include <vector>

struct cpSpace;
struct cpArbiter;

namespace mono
{
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
        IBody* QueryFirst(const math::Vector& start, const math::Vector& end);
        
    private:

        bool OnCollision(cpArbiter* arb);
        void DoForEachFuncOnBody(cpBody* body);

        cpSpace* m_space;
        BodyFunc m_for_each_func;
        std::vector<IBody*> m_bodies;
    };
}
