//
//  PhysicsZone.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "ZoneBase.h"
#include "CMFwd.h"

namespace mono
{
    class PhysicsZone : public ZoneBase
    {
    protected:
        
        PhysicsZone(const Math::Vector2f& gravity);
        virtual void Accept(IRenderer& renderer);
        
        void AddBody(cm::IBodyPtr body);
        void AddShape(cm::IShapePtr shape);
        void AddPhysicsObject(cm::Object& object, bool addBody);
        
    private:
        
        struct PhysicsImpl;
        std::tr1::shared_ptr<PhysicsImpl> mPhysics;
    };

}

