//
//  CMSpace.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "CMFwd.h"
#include "Math/MathFwd.h"
#include <vector>
#include <functional>

struct cpSpace;
struct cpArbiter;

namespace mono
{
    class Space
    {
    public:
        
        Space(const math::Vector& gravity, float damping);
        ~Space();
        
        void Tick(float delta);
        
        void AddBody(const IBodyPtr& body);
        void RemoveBody(const IBodyPtr& body);
        
        void AddShape(const IShapePtr& shape);
        void RemoveShape(const IShapePtr& shape);
        
        void ForEachBody(const BodyFunc& func);
        IBodyPtr QueryFirst(const math::Vector& start, const math::Vector& end);
        
    private:
        
        bool OnCollision(cpArbiter* arb);
        void OnPostStep(cpArbiter* arb);
        
        void DoForEachFuncOnBody(cpBody* body);

        BodyFunc mForEachFunc;
        
        cpSpace* mSpace;
        std::vector<IBodyPtr> mBodies;        
    };
    
}
