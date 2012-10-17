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

namespace
{
    BodyFunc bodyFunction;
    std::vector<IBodyPtr>* bodyCollection;
    
    void CMBodyFunction(cpBody* body, void* data)
    {
        for(std::vector<IBodyPtr>::iterator it = bodyCollection->begin(), end = bodyCollection->end(); it != end; ++it)
        {
            IBodyPtr bodyPtr = *it;
            if(body == bodyPtr->Body())
                bodyFunction(bodyPtr);
        }
    }
}

Space::Space(const Math::Vector2f& gravity)
{
    mSpace = cpSpaceNew();
    cpSpaceSetGravity(mSpace, cpv(gravity.mX, gravity.mY));
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
    cpSpaceAddBody(mSpace, body->Body());
    mBodies.push_back(body);
}
void Space::RemoveBody(IBodyPtr body)
{
    cpSpaceRemoveBody(mSpace, body->Body());
    const bool result = mono::FindAndRemove(mBodies, body);
    if(!result)
        throw std::runtime_error("Unable to remove body from collection");
}

void Space::AddShape(IShapePtr shape)
{
    if(shape->IsStatic())
        cpSpaceAddStaticShape(mSpace, shape->Shape());
    else
        cpSpaceAddShape(mSpace, shape->Shape());
}
void Space::RemoveShape(IShapePtr shape)
{
    if(shape->IsStatic())
        cpSpaceRemoveStaticShape(mSpace, shape->Shape());
    else
        cpSpaceRemoveShape(mSpace, shape->Shape());
}

void Space::ForEachBody(const BodyFunc& func)
{
    bodyFunction = func;
    bodyCollection = &mBodies;
    
    cpSpaceEachBody(mSpace, CMBodyFunction, 0);
}


