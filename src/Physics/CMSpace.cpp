
#include "CMSpace.h"
#include "IBody.h"
#include "IShape.h"
#include "IConstraint.h"
#include "Math/MathFwd.h"
#include "Math/Vector.h"
#include "Util/Algorithm.h"
#include "System/System.h"
#include "chipmunk/chipmunk.h"
#include <algorithm>
#include <cstdio>

using namespace mono;

PhysicsSpace::PhysicsSpace()
    : PhysicsSpace(math::Vector(0.0f, 0.0f), 1.0f)
{ }

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

void PhysicsSpace::Tick(uint32_t delta_ms)
{
    cpSpaceStep(m_space, float(delta_ms) / 1000.0f);
}

void PhysicsSpace::SetGravity(const math::Vector& gravity)
{
    cpSpaceSetGravity(m_space, cpv(gravity.x, gravity.y));
}

void PhysicsSpace::SetDamping(float damping)
{
    cpSpaceSetDamping(m_space, damping);
}

void PhysicsSpace::Add(IBody* body)
{
    if(body->GetType() != mono::BodyType::STATIC)
        cpSpaceAddBody(m_space, body->Handle());
    m_bodies.push_back(body);
}

void PhysicsSpace::Remove(IBody* body)
{
    if(body->GetType() != mono::BodyType::STATIC)
        cpSpaceRemoveBody(m_space, body->Handle());

    const bool removed = mono::remove(m_bodies, body);
    if(!removed)
        System::Log("PhysicsSpace|Unable to remove body from collection!\n");
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

mono::IBody* PhysicsSpace::QueryFirst(const math::Vector& start, const math::Vector& end, uint32_t category)
{
    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, category, CP_ALL_CATEGORIES);
    const cpShape* shape = cpSpaceSegmentQueryFirst(m_space, cpv(start.x, start.y), cpv(end.x, end.y), 1, shape_filter, nullptr);
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

std::vector<IBody*> PhysicsSpace::QueryAllInLIne(const math::Vector& start, const math::Vector& end, float max_distance, uint32_t category)
{
    std::vector<cpBody*> found_bodies;

    const cpSpaceSegmentQueryFunc query_func = [](cpShape* shape, cpVect point, cpVect normal, cpFloat alpha, void* data) {
        std::vector<cpBody*>* found_bodies = (std::vector<cpBody*>*)data;
        found_bodies->push_back(cpShapeGetBody(shape));
    };

    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, category, CP_ALL_CATEGORIES);
    cpSpaceSegmentQuery(m_space, cpv(start.x, start.y), cpv(end.x, end.y), 0.1f, shape_filter, query_func, &found_bodies);

    std::vector<IBody*> bodies;
    bodies.reserve(found_bodies.size());

    for(const cpBody* cpbody : found_bodies)
    {
        const auto find_body_func = [cpbody](IBody* bodyPtr) {
            return bodyPtr->Handle() == cpbody;
        };

        const auto& it = std::find_if(m_bodies.begin(), m_bodies.end(), find_body_func);
        if(it != m_bodies.end())
            bodies.push_back(*it);
    }

    return bodies;
}

IBody* PhysicsSpace::QueryNearest(const math::Vector& point, float max_distance, uint32_t category)
{
    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, category, CP_ALL_CATEGORIES);

    cpPointQueryInfo info;
    const cpShape* shape = cpSpacePointQueryNearest(m_space, cpv(point.x, point.y), max_distance, shape_filter, &info);
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

IBody* PhysicsSpace::QueryNearest(const math::Vector& point, float max_distance, uint32_t category, const QueryFilter& filter_func)
{
    struct UserData
    {
        QueryFilter filter_func;
        float distance;
        const cpBody* cp_body;
    };

    UserData user_data = { filter_func, math::INF, nullptr };

    const auto callback = [](cpShape* shape, cpVect point, cpFloat distance, cpVect gradient, void* data) {
        UserData* user_data = (UserData*)data;

        if(distance > user_data->distance)
            return;

        const cpBody* body = cpShapeGetBody(shape);
        const uint32_t entity_id = reinterpret_cast<uint64_t>(cpBodyGetUserData(body));

        const bool passed_user_filter = user_data->filter_func(entity_id);
        if(!passed_user_filter)
            return;

        user_data->distance = distance;
        user_data->cp_body = body;
    };

    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);
    cpSpacePointQuery(m_space, cpv(point.x, point.y), max_distance, shape_filter, callback, &user_data);

    const auto func = [&user_data](IBody* bodyPtr) {
        return bodyPtr->Handle() == user_data.cp_body;
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
        cpShape* shape1 = nullptr;
        cpShape* shape2 = nullptr;
        cpArbiterGetShapes(arb, &shape1, &shape2);

        const cpShapeFilter& filter1 = cpShapeGetFilter(shape1);
        const cpShapeFilter& filter2 = cpShapeGetFilter(shape2);

        const cpVect point_a = cpArbiterGetPointA(arb, 0);
        const math::Vector collision_point(point_a.x, point_a.y);

        const mono::CollisionResolve resolve1 = first->OnCollideWith(second, collision_point, filter2.categories);
        const mono::CollisionResolve resolve2 = second->OnCollideWith(first, collision_point, filter1.categories);

        if(resolve1 == mono::CollisionResolve::IGNORE || resolve2 == mono::CollisionResolve::IGNORE)
            return false;
    }
    
    return true;
}

cpSpace* PhysicsSpace::Handle()
{
    return m_space;
}
