
#include "CMSpace.h"
#include "CMFactory.h"
#include "IBody.h"
#include "IShape.h"
#include "IConstraint.h"
#include "Math/Vector.h"
#include "Algorithm.h"

#include "chipmunk/chipmunk.h"
#include <algorithm>
#include <cstdio>

using namespace mono;

PhysicsSpace::PhysicsSpace(const math::Vector& gravity, float damping)
    : m_space(cpSpaceNew())
{
    cpSpaceSetGravity(m_space, cpv(gravity.x, gravity.y));
    cpSpaceSetDamping(m_space, damping);

    const auto beginFunc = [](cpArbiter* arb, cpSpace*, cpDataPointer data) -> cpBool {
        return static_cast<PhysicsSpace*>(data)->OnCollision(arb);
    };

    cpCollisionHandler* ch = cpSpaceAddDefaultCollisionHandler(m_space);
    ch->beginFunc = beginFunc;
    ch->userData = this;
}

PhysicsSpace::~PhysicsSpace()
{
    m_bodies.clear();
    cpSpaceDestroy(m_space);
}

void PhysicsSpace::Tick(unsigned int delta)
{
    const float float_delta = delta;
    cpSpaceStep(m_space, float_delta / 1000.0f);
}

void PhysicsSpace::Add(IBody* body)
{
    if(!body->IsStatic())
        cpSpaceAddBody(m_space, body->Handle());
    m_bodies.push_back(body);
}

void PhysicsSpace::Remove(IBody* body)
{
    if(!body->IsStatic())
        cpSpaceRemoveBody(m_space, body->Handle());

    const bool removed = mono::remove(m_bodies, body);
    if(!removed)
        std::printf("Unable to remove body from collection!\n");
}

void PhysicsSpace::Add(IShape* shape)
{
    cpSpaceAddShape(m_space, shape->Handle());
}

void PhysicsSpace::Remove(IShape* shape)
{
    cpSpaceRemoveShape(m_space, shape->Handle());
}

void PhysicsSpace::Add(IConstraint* constraint)
{
    cpSpaceAddConstraint(m_space, constraint->Handle());
}

void PhysicsSpace::Remove(IConstraint* constraint)
{
    cpSpaceRemoveConstraint(m_space, constraint->Handle());
}

void PhysicsSpace::ForEachBody(const BodyFunc& func)
{
    const auto forEachBody = [](cpBody* body, void* data) {
        static_cast<PhysicsSpace*>(data)->DoForEachFuncOnBody(body);
    };
    
    m_for_each_func = func;
    cpSpaceEachBody(m_space, forEachBody, this);
    
    // Null the function, so it wont be used by misstake after this point
    m_for_each_func = nullptr;
}

void PhysicsSpace::DoForEachFuncOnBody(cpBody* body)
{
    for(auto& bodyPtr : m_bodies)
    {
        if(body == bodyPtr->Handle())
        {
            m_for_each_func(bodyPtr);
            break;
        }
    }
}

mono::IBody* PhysicsSpace::QueryFirst(const math::Vector& start, const math::Vector& end)
{
    const cpShape* shape = cpSpaceSegmentQueryFirst(m_space, cpv(start.x, start.y), cpv(end.x, end.y), 1, CP_SHAPE_FILTER_ALL, nullptr);
    if(!shape)
        return nullptr;

    const cpBody* body = cpShapeGetBody(shape);

    const auto func = [body](IBody* bodyPtr) {
        return bodyPtr->Handle() == body;
    };

    const auto& it = std::find_if(m_bodies.begin(), m_bodies.end(), func);
    if(it == m_bodies.end())
        return nullptr;

    return *it;
}

bool PhysicsSpace::OnCollision(cpArbiter* arb)
{
    cpBody* b1 = nullptr;
    cpBody* b2 = nullptr;
    cpArbiterGetBodies(arb, &b1, &b2);

    cpShape* shape1 = nullptr;
    cpShape* shape2 = nullptr;
    cpArbiterGetShapes(arb, &shape1, &shape2);

    const cpShapeFilter& filter1 = cpShapeGetFilter(shape1);
    const cpShapeFilter& filter2 = cpShapeGetFilter(shape2);
    
    IBody* first = nullptr;
    IBody* second = nullptr;
    
    for(auto& body : m_bodies)
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
        first->OnCollideWith(second, filter2.categories);
        second->OnCollideWith(first, filter1.categories);
    }
    
    return true;
}
