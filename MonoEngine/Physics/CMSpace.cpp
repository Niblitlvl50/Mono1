//
//  CMSpace.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CMSpace.h"
#include "CMFactory.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "Vector2f.h"
#include "Utils.h"

#include "chipmunk.h"

#include <stdexcept>

using namespace cm;


Space::Space(const math::Vector2f& gravity, float damping)
{
    const auto beginFunc = [](cpArbiter* arb, cpSpace* space, void* data) -> cpBool {
        return static_cast<Space*>(data)->OnCollision(arb);
    };

    mSpace = cpSpaceNew();
    cpSpaceSetGravity(mSpace, cpv(gravity.mX, gravity.mY));
    cpSpaceSetDamping(mSpace, damping);
    cpSpaceAddCollisionHandler(mSpace, 0, 0, beginFunc, 0, 0, 0, this);
}

Space::~Space()
{
    cpSpaceDestroy(mSpace);
}

void Space::Tick(float delta)
{
    cpSpaceStep(mSpace, delta);
}

void Space::AddBody(IBodyPtr body)
{
    if(!body->IsStatic())
        cpSpaceAddBody(mSpace, body->Body());
    mBodies.push_back(body);
}

void Space::RemoveBody(IBodyPtr body)
{
    if(!body->IsStatic())
        cpSpaceRemoveBody(mSpace, body->Body());
    
    const bool result = mono::FindAndRemove(mBodies, body);
    if(!result)
        throw std::runtime_error("Unable to remove body from collection");
}

void Space::AddShape(IShapePtr shape)
{
    cpSpaceAddShape(mSpace, shape->Shape());
}

void Space::RemoveShape(IShapePtr shape)
{
    cpSpaceRemoveShape(mSpace, shape->Shape());
}

void Space::ForEachBody(const std::function<void (IBodyPtr)>& func)
{
    const auto forEachBody = [](cpBody* body, void* data) {
        static_cast<Space*>(data)->DoForEachFuncOnBody(body);
    };
    
    mForEachFunc = func;
    cpSpaceEachBody(mSpace, forEachBody, this);
    
    // Null the function, so it wont be used by misstake after this point
    mForEachFunc = nullptr;
}

void Space::DoForEachFuncOnBody(cpBody* body)
{
    for(auto& bodyPtr : mBodies)
    {
        if(body == bodyPtr->Body())
        {
            mForEachFunc(bodyPtr);
            break;
        }
    }
}

bool Space::OnCollision(cpArbiter* arb)
{
    cpBody* b1 = nullptr;
    cpBody* b2 = nullptr;
    cpArbiterGetBodies(arb, &b1, &b2);
    
    IBodyPtr first = nullptr;
    IBodyPtr second = nullptr;
    
    for(auto& body : mBodies)
    {
        if(body->Body() == b1)
            first = body;
        else if(body->Body() == b2)
            second = body;
        
        if(first && second)
            break;
    }
    
    if(first && second)
    {
        first->OnCollideWith(second);
        second->OnCollideWith(first);
    }
    
    return true;
}

void Space::OnPostStep(void* object, void* data)
{
    for(auto& body : mBodies)
    {
        if(object == body->Body())
        {
            body->OnPostStep();
            break;
        }
    }
}




