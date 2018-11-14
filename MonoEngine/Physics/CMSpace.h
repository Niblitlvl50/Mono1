
#pragma once

#include "CMFwd.h"
#include "Math/MathFwd.h"
#include "Zone/IPhysicsZone.h"
#include <vector>

struct cpSpace;
struct cpArbiter;

namespace mono
{
    class PhysicsSpace
    {
    public:
        
        PhysicsSpace(const math::Vector& gravity, float damping);
        ~PhysicsSpace();
        
        void Tick(unsigned int delta);
        
        void Add(const IBodyPtr& body);
        void Add(const IShapePtr& shape);
        void Add(const IConstraintPtr& constraint);
        
        void Remove(const IBodyPtr& body);
        void Remove(const IShapePtr& shape);
        void Remove(const IConstraintPtr& constraint);
        
        void ForEachBody(const BodyFunc& func);
        IBodyPtr QueryFirst(const math::Vector& start, const math::Vector& end);
        
    private:
        
        bool OnCollision(cpArbiter* arb);        
        void DoForEachFuncOnBody(cpBody* body);

        BodyFunc m_for_each_func;
        
        cpSpace* m_space;
        std::vector<IBodyPtr> m_bodies;
    };
}
