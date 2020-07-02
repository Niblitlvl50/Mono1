
#include "CMSpace.h"
#include "IBody.h"
#include "IShape.h"
#include "IConstraint.h"
#include "Impl/BodyImpl.h"
#include "Physics/PhysicsSystem.h"
#include "Math/Vector.h"

#include "chipmunk/chipmunk.h"

#include <cstdio>

using namespace mono;

PhysicsSpace::PhysicsSpace(PhysicsSystem* physics_system)
    : PhysicsSpace(physics_system, math::Vector(0.0f, 0.0f), 1.0f)
{ }

PhysicsSpace::PhysicsSpace(PhysicsSystem* physics_system, const math::Vector& gravity, float damping)
    : m_physics_system(physics_system)
    , m_space(cpSpaceNew())
{
    cpSpaceSetGravity(m_space, cpv(gravity.x, gravity.y));
    cpSpaceSetDamping(m_space, damping);

    const auto begin_func = [](cpArbiter* arb, cpSpace* space, cpDataPointer user_data) -> cpBool {
        PhysicsSpace* physics_space = static_cast<PhysicsSpace*>(user_data);
        return physics_space->OnCollision(arb);
    };

    const auto separate_func = [](cpArbiter* arb, cpSpace* space, cpDataPointer user_data) {
        PhysicsSpace* physics_space = static_cast<PhysicsSpace*>(user_data);
        physics_space->OnSeparation(arb);
    };

    cpCollisionHandler* ch = cpSpaceAddDefaultCollisionHandler(m_space);
    ch->beginFunc = begin_func;
    ch->separateFunc = separate_func;
    ch->userData = this;

    m_static_body = std::make_unique<cm::BodyImpl>(cpSpaceGetStaticBody(m_space));
}

PhysicsSpace::~PhysicsSpace()
{
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
}

void PhysicsSpace::Remove(IBody* body)
{
    if(body->GetType() != mono::BodyType::STATIC)
        cpSpaceRemoveBody(m_space, body->Handle());
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

mono::IBody* PhysicsSpace::QueryFirst(const math::Vector& start, const math::Vector& end, uint32_t category)
{
    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, category, CP_ALL_CATEGORIES);
    const cpShape* shape = cpSpaceSegmentQueryFirst(m_space, cpv(start.x, start.y), cpv(end.x, end.y), 1, shape_filter, nullptr);
    if(!shape)
        return nullptr;

    const cpBody* body = cpShapeGetBody(shape);
    const uint32_t body_id = reinterpret_cast<uint64_t>(cpBodyGetUserData(body));
    return m_physics_system->GetBody(body_id);
}

std::vector<IBody*> PhysicsSpace::QueryAllInLIne(const math::Vector& start, const math::Vector& end, float max_distance, uint32_t category)
{
    std::vector<cpBody*> found_bodies;

    const cpSpaceSegmentQueryFunc query_func = [](cpShape* shape, cpVect point, cpVect normal, cpFloat alpha, void* data) {
        std::vector<cpBody*>* found_bodies = (std::vector<cpBody*>*)data;
        found_bodies->push_back(cpShapeGetBody(shape));
    };

    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);
    cpSpaceSegmentQuery(m_space, cpv(start.x, start.y), cpv(end.x, end.y), 0.1f, shape_filter, query_func, &found_bodies);

    std::vector<IBody*> bodies;
    bodies.reserve(found_bodies.size());

    for(const cpBody* cpbody : found_bodies)
    {
        const uint32_t body_id = reinterpret_cast<uint64_t>(cpBodyGetUserData(cpbody));
        bodies.push_back(m_physics_system->GetBody(body_id));
    }

    return bodies;
}

IBody* PhysicsSpace::QueryNearest(const math::Vector& point, float max_distance, uint32_t category)
{
    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);

    cpPointQueryInfo info;
    const cpShape* shape = cpSpacePointQueryNearest(m_space, cpv(point.x, point.y), max_distance, shape_filter, &info);
    if(!shape)
        return nullptr;

    const cpBody* cpbody = cpShapeGetBody(shape);
    const uint32_t body_id = reinterpret_cast<uint64_t>(cpBodyGetUserData(cpbody));
    return m_physics_system->GetBody(body_id);
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

        const bool passed_user_filter = user_data->filter_func(entity_id, math::Vector(point.x, point.y));
        if(!passed_user_filter)
            return;

        user_data->distance = distance;
        user_data->cp_body = body;
    };

    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);
    cpSpacePointQuery(m_space, cpv(point.x, point.y), max_distance, shape_filter, callback, &user_data);

    if(!user_data.cp_body)
        return nullptr;

    const uint32_t body_id = reinterpret_cast<uint64_t>(cpBodyGetUserData(user_data.cp_body));
    return m_physics_system->GetBody(body_id);
}

bool PhysicsSpace::OnCollision(cpArbiter* arb)
{
    cpBody* b1 = nullptr;
    cpBody* b2 = nullptr;
    cpArbiterGetBodies(arb, &b1, &b2);

    const uint32_t body_id_1 = reinterpret_cast<uint64_t>(cpBodyGetUserData(b1));
    const uint32_t body_id_2 = reinterpret_cast<uint64_t>(cpBodyGetUserData(b2));

    IBody* first = m_physics_system->GetBody(body_id_1);
    IBody* second = m_physics_system->GetBody(body_id_2);
    
    if(first && second)
    {
        cpShape* shape1 = nullptr;
        cpShape* shape2 = nullptr;
        cpArbiterGetShapes(arb, &shape1, &shape2);

        const bool is_shape_1_sensor = cpShapeGetSensor(shape1);
        const bool is_shape_2_sensor = cpShapeGetSensor(shape2);

        const cpShapeFilter& filter1 = cpShapeGetFilter(shape1);
        const cpShapeFilter& filter2 = cpShapeGetFilter(shape2);

        const cpVect point_a = cpArbiterGetPointA(arb, 0);
        const math::Vector collision_point(point_a.x, point_a.y);

        if(is_shape_1_sensor)
            return (first->OnCollideWith(second, collision_point, filter2.categories) != mono::CollisionResolve::IGNORE);
        else if(is_shape_2_sensor)
            return (second->OnCollideWith(first, collision_point, filter1.categories) != mono::CollisionResolve::IGNORE);

        const mono::CollisionResolve resolve1 = first->OnCollideWith(second, collision_point, filter2.categories);
        const mono::CollisionResolve resolve2 = second->OnCollideWith(first, collision_point, filter1.categories);
        if(resolve1 == mono::CollisionResolve::IGNORE || resolve2 == mono::CollisionResolve::IGNORE)
            return false;
    }
    
    return true;
}

void PhysicsSpace::OnSeparation(cpArbiter* arb)
{
    cpBody* b1 = nullptr;
    cpBody* b2 = nullptr;
    cpArbiterGetBodies(arb, &b1, &b2);

    const uint32_t body_id_1 = reinterpret_cast<uint64_t>(cpBodyGetUserData(b1));
    const uint32_t body_id_2 = reinterpret_cast<uint64_t>(cpBodyGetUserData(b2));

    IBody* first = m_physics_system->GetBody(body_id_1);
    IBody* second = m_physics_system->GetBody(body_id_2);

    if(first && second)
    {
        first->OnSeparateFrom(second);
        second->OnSeparateFrom(first);
    }
}

IBody* PhysicsSpace::GetStaticBody()
{
    return m_static_body.get();
}

cpSpace* PhysicsSpace::Handle()
{
    return m_space;
}
