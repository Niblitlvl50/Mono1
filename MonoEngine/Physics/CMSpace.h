//
//  CMSpace.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "CMFwd.h"
#include "MathFwd.h"
#include <vector>
#include <functional>

struct cpSpace;
struct cpArbiter;

namespace cm
{
    class Space
    {
    public:
        
        Space(const math::Vector2f& gravity, float damping = 1.0f);
        ~Space();
        
        void Tick(float delta);
        
        void AddBody(IBodyPtr body);
        void RemoveBody(IBodyPtr body);
        
        void AddShape(IShapePtr shape);
        void RemoveShape(IShapePtr shape);
        
        void ForEachBody(const std::function<void (IBodyPtr)>& func);
        
    private:
        
        bool OnCollision(cpArbiter* arb);
        void OnPostStep(void* object, void* data);
        
        void DoForEachFuncOnBody(cpBody* body);
        std::function<void (IBodyPtr)> mForEachFunc;
        
        cpSpace* mSpace;
        std::vector<IBodyPtr> mBodies;        
    };
    
}
