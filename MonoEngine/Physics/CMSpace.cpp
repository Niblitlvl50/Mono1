
#include "CMSpace.h"
#include "CMFactory.h"
#include "IBody.h"
#include "IShape.h"
#include "IConstraint.h"
#include "Math/Vector.h"

#include "chipmunk/chipmunk.h"
#include <cstdio>

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
    mBodies.clear();
    cpSpaceDestroy(mSpace);
}

void Space::Tick(float delta)
{
    cpSpaceStep(mSpace, delta);
}

void Space::Add(const IBodyPtr& body)
{
    if(!body->IsStatic())
        cpSpaceAddBody(mSpace, body->Handle());
    mBodies.push_back(body);
}

void Space::Remove(const IBodyPtr& body)
{
    if(!body->IsStatic())
        cpSpaceRemoveBody(mSpace, body->Handle());

    const auto it = std::find(mBodies.begin(), mBodies.end(), body);
    if(it != mBodies.end())
    {
        mBodies.erase(it);
    }
    else
    {
        std::printf("Unable to remove body from collection!\n");
    }
}

void Space::Add(const IShapePtr& shape)
{
    cpSpaceAddShape(mSpace, shape->Handle());
}

void Space::Remove(const IShapePtr& shape)
{
    cpSpaceRemoveShape(mSpace, shape->Handle());
}

void Space::Add(const IConstraintPtr& constraint)
{
    cpSpaceAddConstraint(mSpace, constraint->Handle());
}

void Space::Remove(const IConstraintPtr& constraint)
{
    cpSpaceRemoveConstraint(mSpace, constraint->Handle());
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
        if(body == bodyPtr->Handle())
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
        return bodyPtr->Handle() == body;
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
        if(body->Handle() == b1)
            first = body;
        else if(body->Handle() == b2)
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
        if(body->Handle() == b1)
            first = body;
        else if(body->Handle() == b2)
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
