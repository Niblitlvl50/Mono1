//
//  CMSpace.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "CMFwd.h"

struct cpArbiter;

namespace cm
{
    class Space
    {
    public:
        
        Space(const Math::Vector2f& gravity);
        ~Space();
        
        void Tick(float delta);
        
        void AddBody(IBodyPtr body);
        void RemoveBody(IBodyPtr body);
        
        void AddShape(IShapePtr shape);
        void RemoveShape(IShapePtr shape);
        
        void ForEachBody(const BodyFunc& func);
        
    private:
        
        bool OnCollision(cpArbiter* arb, void* data);
        
        cpSpace* mSpace;
        std::vector<IBodyPtr> mBodies;
    };
    
}
