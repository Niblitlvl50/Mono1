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
#include "Math/Vector.h"
#include "Utils.h"

#include "chipmunk/chipmunk.h"

#include <stdexcept>

using namespace mono;


Space::Space(const math::Vector& gravity, float damping)
{
    mSpace = cpSpaceNew();
    cpSpaceSetGravity(mSpace, cpv(gravity.x, gravity.y));
    cpSpaceSetDamping(mSpace, damping);

    const auto beginFunc = [](cpArbiter* arb, cpSpace*, cpDataPointer data) -> cpBool {
        return static_cast<Space*>(data)->OnCollision(arb);
    };

    const auto postSolve = [](cpArbiter* arb, cpSpace*, cpDataPointer userData) {
        static_cast<Space*>(userData)->OnPostStep(arb);
    };

    cpCollisionHandler* ch = cpSpaceAddDefaultCollisionHandler(mSpace);
    ch->beginFunc = beginFunc;
    ch->postSolveFunc = postSolve;
    ch->userData = this;
}

Space::~Space()
{
    cpSpaceDestroy(mSpace);
}

void Space::Tick(float delta)
{
    cpSpaceStep(mSpace, delta);
}

void Space::AddBody(const IBodyPtr& body)
{
    if(!body->IsStatic())
        cpSpaceAddBody(mSpace, body->Body());
    mBodies.push_back(body);
}

void Space::RemoveBody(const IBodyPtr& body)
{
    if(!body->IsStatic())
        cpSpaceRemoveBody(mSpace, body->Body());
    
    const bool result = mono::FindAndRemove(mBodies, body);
    if(!result)
        throw std::runtime_error("Unable to remove body from collection");
}

void Space::AddShape(const IShapePtr& shape)
{
    cpSpaceAddShape(mSpace, shape->Shape());
}

void Space::RemoveShape(const IShapePtr& shape)
{
    cpSpaceRemoveShape(mSpace, shape->Shape());
}

void Space::ForEachBody(const BodyFunc& func)
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

IBodyPtr Space::QueryFirst(const math::Vector& start, const math::Vector& end)
{
    const cpShape* shape = cpSpaceSegmentQueryFirst(mSpace, cpv(start.x, start.y), cpv(end.x, end.y), 1, CP_SHAPE_FILTER_ALL, nullptr);
    if(!shape)
        return nullptr;

    const cpBody* body = cpShapeGetBody(shape);

    const auto func = [body](const IBodyPtr& bodyPtr) {
        return bodyPtr->Body() == body;
    };

    const auto& it = std::find_if(mBodies.begin(), mBodies.end(), func);
    if(it == mBodies.end())
        return nullptr;

    return *it;
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

void Space::OnPostStep(cpArbiter* arb)
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
        first->OnPostStep();
        second->OnPostStep();
    }
}




