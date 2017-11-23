
#pragma once

#include "CMFwd.h"
#include "Math/MathFwd.h"
#include <vector>

struct cpSpace;
struct cpArbiter;

namespace mono
{
    class Space
    {
    public:
        
        Space(const math::Vector& gravity, float damping);
        ~Space();
        
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

        BodyFunc mForEachFunc;
        
        cpSpace* mSpace;
        std::vector<IBodyPtr> mBodies;
    };
}
