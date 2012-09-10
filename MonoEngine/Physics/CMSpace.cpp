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

#include "chipmunk.h"

using namespace cm;

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
}
void Space::RemoveBody(IBodyPtr body)
{
    cpSpaceRemoveBody(mSpace, body->Body());
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



