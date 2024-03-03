
#include "PhysicsSpace.h"
#include "IBody.h"
#include "IShape.h"
#include "IConstraint.h"
#include "Impl/BodyImpl.h"
#include "Physics/PhysicsSystem.h"
#include "Math/Vector.h"
#include "Math/Quad.h"
#include "System/System.h"

#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_private.h"

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

    m_static_body = std::make_unique<cm::BodyImpl>(-1, cpSpaceGetStaticBody(m_space));
}

PhysicsSpace::~PhysicsSpace()
{
    cpSpaceDestroy(m_space);
}

void PhysicsSpace::Tick(float delta_s)
{
    cpSpaceStep(m_space, delta_s);
}

void PhysicsSpace::SetGravity(const math::Vector& gravity)
{
    cpSpaceSetGravity(m_space, cpv(gravity.x, gravity.y));
}

void PhysicsSpace::SetDamping(float damping)
{
    cpSpaceSetDamping(m_space, damping);
}

float PhysicsSpace::GetTimeStep() const
{
    return cpSpaceGetCurrentTimeStep(m_space);
}

void PhysicsSpace::Add(IBody* body)
{
    cpSpaceAddBody(m_space, body->Handle());
}

void PhysicsSpace::Remove(IBody* body)
{
    const bool contains_body = cpSpaceContainsBody(m_space, body->Handle());
    if(contains_body)
    {
        if(body->GetType() != mono::BodyType::STATIC)
            cpSpaceRemoveBody(m_space, body->Handle());
    }
    else
    {
        const uint32_t body_id = PhysicsSystem::GetIdFromBody(body);
        System::Log("physics|Trying to remove body that's not added to the space. [%u]", body_id);
    }
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

QueryResult PhysicsSpace::QueryFirst(const math::Vector& start, const math::Vector& end, uint32_t category) const
{
    QueryResult result = {};

    cpSegmentQueryInfo query_info;

    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);
    const cpShape* shape = cpSpaceSegmentQueryFirst(m_space, cpv(start.x, start.y), cpv(end.x, end.y), 0.0f, shape_filter, &query_info);
    if(!shape)
        return result;

    const cpBody* cp_body = cpShapeGetBody(shape);
    result.body = (mono::IBody*)cpBodyGetUserData(cp_body);
    result.point = math::Vector(query_info.point.x, query_info.point.y);

    const cpShapeFilter filter = cpShapeGetFilter(shape);
    result.collision_category = filter.categories;

    return result;
}

std::vector<QueryResult> PhysicsSpace::QueryAllInLIne(const math::Vector& start, const math::Vector& end, float max_distance, uint32_t category) const
{
    struct QueryData
    {
        PhysicsSystem* physics_system;
        std::vector<QueryResult> found_bodies;
    };

    QueryData query_data;
    query_data.physics_system = m_physics_system;

    const cpSpaceSegmentQueryFunc query_func = [](cpShape* shape, cpVect point, cpVect normal, cpFloat alpha, void* data) {
        const cpBody* cm_body = cpShapeGetBody(shape);
        mono::IBody* body = (mono::IBody*)cpBodyGetUserData(cm_body);

        const cpShapeFilter filter = cpShapeGetFilter(shape);

        QueryData* query_data = (QueryData*)data;
 
        const QueryResult query_result = {
            body, body->GetPosition(), filter.categories
        };
        query_data->found_bodies.push_back(query_result);
    };

    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);
    cpSpaceSegmentQuery(m_space, cpv(start.x, start.y), cpv(end.x, end.y), 0.1f, shape_filter, query_func, &query_data);

    return query_data.found_bodies;
}

std::vector<QueryResult> PhysicsSpace::QueryBox(const math::Quad& world_bb, uint32_t category) const
{
    struct QueryData
    {
        PhysicsSystem* physics_system;
        std::vector<QueryResult> found_bodies;
    };

    QueryData query_data;
    query_data.physics_system = m_physics_system;

    const cpSpaceBBQueryFunc query_func = [](cpShape* shape, void* data) {
        const cpBody* cm_body = cpShapeGetBody(shape);
        mono::IBody* body = (mono::IBody*)cpBodyGetUserData(cm_body);

        const cpShapeFilter filter = cpShapeGetFilter(shape);
        QueryData* query_data = (QueryData*)data;

        const QueryResult query_result = {
            body, body->GetPosition(), filter.categories
        };
        query_data->found_bodies.push_back(query_result);
    };

    const float left = std::min(math::Left(world_bb), math::Right(world_bb));
    const float bottom = std::min(math::Bottom(world_bb), math::Top(world_bb));
    const float right = std::max(math::Left(world_bb), math::Right(world_bb));
    const float top = std::max(math::Bottom(world_bb), math::Top(world_bb));

    const cpBB bounding_box = cpBBNew(left, bottom, right, top);
    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);
    cpSpaceBBQuery(m_space, bounding_box, shape_filter, query_func, &query_data);

    return query_data.found_bodies;

/*
/// Rectangle Query callback function type.
typedef void (*cpSpaceBBQueryFunc)(cpShape *shape, void *data);
/// Perform a fast rectangle query on the space calling @c func for each shape found.
/// Only the shape's bounding boxes are checked for overlap, not their full shape.
CP_EXPORT void cpSpaceBBQuery(cpSpace *space, cpBB bb, cpShapeFilter filter, cpSpaceBBQueryFunc func, void *data);
*/
}

std::vector<QueryResult> PhysicsSpace::QueryRadius(const math::Vector& position, float radius, uint32_t category) const
{
    cpCircleShape circle_shape;
    cpCircleShapeInit(&circle_shape, cpSpaceGetStaticBody(m_space), radius, cpv(position.x, position.y));

    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);
    cpShapeSetFilter((cpShape*)&circle_shape, shape_filter);


    struct QueryData
    {
        PhysicsSystem* physics_system;
        std::vector<QueryResult> found_bodies;
    };

    QueryData query_data;
    query_data.physics_system = m_physics_system;

    const cpSpaceShapeQueryFunc query_func = [](cpShape* shape, cpContactPointSet* points, void* data) {
        const cpBody* cm_body = cpShapeGetBody(shape);
        mono::IBody* body = (mono::IBody*)cpBodyGetUserData(cm_body);

        const cpShapeFilter filter = cpShapeGetFilter(shape);

        QueryData* query_data = (QueryData*)data;
        const QueryResult query_result = {
            body, body->GetPosition(), filter.categories
        };
        query_data->found_bodies.push_back(query_result);
    };

    cpSpaceShapeQuery(m_space, (cpShape*)&circle_shape, query_func, &query_data);

    return query_data.found_bodies;
}

QueryResult PhysicsSpace::QueryNearest(const math::Vector& point, float max_distance, uint32_t category) const
{
    QueryResult result = {};

    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);

    cpPointQueryInfo info;
    const cpShape* shape = cpSpacePointQueryNearest(m_space, cpv(point.x, point.y), max_distance, shape_filter, &info);
    if(!shape)
        return result;

    const cpBody* cp_body = cpShapeGetBody(shape);
    mono::IBody* body = (mono::IBody*)cpBodyGetUserData(cp_body);

    result.body = body;
    
    const cpShapeFilter filter = cpShapeGetFilter(shape);
    result.collision_category = filter.categories;

    return result;
}

QueryResult PhysicsSpace::QueryNearest(const math::Vector& point, float max_distance, uint32_t category, const QueryFilter& filter_func) const
{
    struct UserData
    {
        QueryFilter filter_func;
        float distance;
        const cpBody* cp_body;
        const cpShape* cp_shape;
    };

    UserData user_data = { filter_func, math::INF, nullptr, nullptr };

    const auto callback = [](cpShape* shape, cpVect point, cpFloat distance, cpVect gradient, void* data) {
        UserData* user_data = (UserData*)data;

        if(distance > user_data->distance)
            return;

        const cpBody* cm_body = cpShapeGetBody(shape);
        mono::IBody* body = (mono::IBody*)cpBodyGetUserData(cm_body);

        const bool passed_user_filter = user_data->filter_func(body->GetId(), math::Vector(point.x, point.y));
        if(!passed_user_filter)
            return;

        user_data->distance = distance;
        user_data->cp_body = cm_body;
        user_data->cp_shape = shape;
    };

    const cpShapeFilter shape_filter = cpShapeFilterNew(CP_NO_GROUP, CP_ALL_CATEGORIES, category);
    cpSpacePointQuery(m_space, cpv(point.x, point.y), max_distance, shape_filter, callback, &user_data);

    QueryResult result = {};
    if(user_data.cp_body)
    {
        result.body = (mono::IBody*)cpBodyGetUserData(user_data.cp_body);
        const cpShapeFilter filter = cpShapeGetFilter(user_data.cp_shape);
        result.collision_category = filter.categories;
    }

    return result;
}

bool PhysicsSpace::OnCollision(cpArbiter* arb)
{
    cpBody* b1 = nullptr;
    cpBody* b2 = nullptr;
    cpArbiterGetBodies(arb, &b1, &b2);

    IBody* first = (mono::IBody*)cpBodyGetUserData(b1);
    IBody* second = (mono::IBody*)cpBodyGetUserData(b2);

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

        const cpVect normal_arb = cpArbiterGetNormal(arb);
        const math::Vector collision_normal(normal_arb.x, normal_arb.y);

        if(is_shape_1_sensor)
            return (first->OnCollideWith(second, collision_point, -collision_normal, filter2.categories) != mono::CollisionResolve::IGNORE);
        else if(is_shape_2_sensor)
            return (second->OnCollideWith(first, collision_point, collision_normal, filter1.categories) != mono::CollisionResolve::IGNORE);

        const mono::CollisionResolve resolve1 = first->OnCollideWith(second, collision_point, -collision_normal, filter2.categories);
        const mono::CollisionResolve resolve2 = second->OnCollideWith(first, collision_point, collision_normal, filter1.categories);
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

    IBody* first = (mono::IBody*)cpBodyGetUserData(b1);
    IBody* second = (mono::IBody*)cpBodyGetUserData(b2);

    if(first && second)
    {
        first->OnSeparateFrom(second);
        second->OnSeparateFrom(first);
    }
}

void PhysicsSpace::UpdateBodyShapes(IBody* body)
{
    cpSpaceReindexShapesForBody(m_space, body->Handle());
}

IBody* PhysicsSpace::GetStaticBody()
{
    return m_static_body.get();
}

cpSpace* PhysicsSpace::Handle()
{
    return m_space;
}
