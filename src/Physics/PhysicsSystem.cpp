
#include "PhysicsSystem.h"
#include "TransformSystem/TransformSystem.h"
#include "CMSpace.h"
#include "Util/Hash.h"

#include "Impl/BodyImpl.h"
#include "Impl/ShapeImpl.h"
#include "Impl/ConstraintImpl.h"

#include "Math/MathFunctions.h"
#include "Util/ObjectPool.h"
#include "System/System.h"

#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_private.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdio>
#include <unordered_map>
#include <cassert>

using namespace mono;

static_assert(static_cast<int>(BodyType::DYNAMIC) == cpBodyType::CP_BODY_TYPE_DYNAMIC);
static_assert(static_cast<int>(BodyType::KINEMATIC) == cpBodyType::CP_BODY_TYPE_KINEMATIC);
static_assert(static_cast<int>(BodyType::STATIC) == cpBodyType::CP_BODY_TYPE_STATIC);

struct PhysicsSystem::Impl
{
    Impl(const PhysicsSystemInitParams& init_params)
        : body_pool(init_params.n_bodies)
        , circle_shape_pool(init_params.n_circle_shapes)
        , segment_shape_pool(init_params.n_segment_shapes)
        , poly_shape_pool(init_params.n_polygon_shapes)
        , pivot_joint_pool(init_params.n_pivot_joints)
        , gear_joint_pool(init_params.n_gear_joints)
        , damped_spring_pool(init_params.n_damped_springs)
        , shapes(init_params.n_circle_shapes + init_params.n_segment_shapes + init_params.n_polygon_shapes)
        , constraints(init_params.n_pivot_joints + init_params.n_gear_joints + init_params.n_damped_springs)
    { }

    void ReleaseCircleShape(cpShape* shape)
    {
        circle_shape_pool.ReleasePoolData((cpCircleShape*)shape);
    }

    void ReleaseSegmentShape(cpShape* shape)
    {
        segment_shape_pool.ReleasePoolData((cpSegmentShape*)shape);
    }

    void ReleasePolyShape(cpShape* shape)
    {
        poly_shape_pool.ReleasePoolData((cpPolyShape*)shape);
    }

    void ReleaseSpring(cpConstraint* constraint)
    {
        damped_spring_pool.ReleasePoolData((cpDampedSpring*)constraint);
    }

    mono::ObjectPool<cpBody> body_pool;

    mono::ObjectPool<cpCircleShape> circle_shape_pool;
    mono::ObjectPool<cpSegmentShape> segment_shape_pool;
    mono::ObjectPool<cpPolyShape> poly_shape_pool;

    mono::ObjectPool<cpPivotJoint> pivot_joint_pool;
    mono::ObjectPool<cpGearJoint> gear_joint_pool;
    mono::ObjectPool<cpDampedSpring> damped_spring_pool;

    mono::ObjectPool<cm::ShapeImpl> shapes;
    mono::ObjectPool<cm::ConstraintImpl> constraints;

    std::vector<cm::BodyImpl> bodies;
    std::vector<std::vector<cm::ShapeImpl*>> bodies_shapes;
    std::vector<bool> active_bodies;

    mono::PhysicsSpace space;

    using ReleaseShapeFunc = void (Impl::*)(cpShape* handle);
    std::unordered_map<cm::ShapeImpl*, ReleaseShapeFunc> m_shape_release_funcs;

    using ReleaseConstraintFunc = void (Impl::*)(cpConstraint* handle);
    std::unordered_map<IConstraint*, ReleaseConstraintFunc> m_constraint_release_funcs;
};

PhysicsSystem::PhysicsSystem(const PhysicsSystemInitParams& init_params, mono::TransformSystem* transform_system)
    : m_impl(std::make_unique<Impl>(init_params))
    , m_transform_system(transform_system)
{
    m_impl->bodies.reserve(init_params.n_bodies);
    for(size_t index = 0; index < init_params.n_bodies; ++index)
    {
        cpBody* body = m_impl->body_pool.GetPoolData();
        m_impl->bodies.emplace_back(body);
    }

    m_impl->bodies_shapes.resize(init_params.n_bodies);
    m_impl->active_bodies.resize(init_params.n_bodies, false);

    System::Log(
        "Physics\n"
        "\tchipmunk version: %s\n"
        "\tbodies: %zu\n"
        "\tshapes: circle %zu, segment %zu, polygon %zu\n"
        "\tjoints: pivot %zu, gear %zu\n"
        "\tsprings: damped %zu\n",
        cpVersionString,
        init_params.n_bodies,
        init_params.n_circle_shapes, init_params.n_segment_shapes, init_params.n_polygon_shapes,
        init_params.n_pivot_joints, init_params.n_gear_joints,
        init_params.n_damped_springs);
}

PhysicsSystem::~PhysicsSystem()
{
    for(size_t index = 0; index < m_impl->body_pool.Size(); ++index)
        cpBodyDestroy(&m_impl->body_pool.Data()[index]);

    for(size_t index = 0; index < m_impl->circle_shape_pool.Size(); ++index)
        cpShapeDestroy((cpShape*)&m_impl->circle_shape_pool.Data()[index]);

    for(size_t index = 0; index < m_impl->segment_shape_pool.Size(); ++index)
        cpShapeDestroy((cpShape*)&m_impl->segment_shape_pool.Data()[index]);

    for(size_t index = 0; index < m_impl->poly_shape_pool.Size(); ++index)
        cpShapeDestroy((cpShape*)&m_impl->poly_shape_pool.Data()[index]);

    for(size_t index = 0; index < m_impl->pivot_joint_pool.Size(); ++index)
        cpConstraintDestroy((cpConstraint*)&m_impl->pivot_joint_pool.Data()[index]);

    for(size_t index = 0; index < m_impl->gear_joint_pool.Size(); ++index)
        cpConstraintDestroy((cpConstraint*)&m_impl->gear_joint_pool.Data()[index]);

    for(size_t index = 0; index < m_impl->damped_spring_pool.Size(); ++index)
        cpConstraintDestroy((cpConstraint*)&m_impl->damped_spring_pool.Data()[index]);
}

mono::IBody* PhysicsSystem::AllocateBody(uint32_t id, const BodyComponent& body_params)
{
    cm::BodyImpl& new_body = m_impl->bodies[id];

    cpBodySetType(new_body.Handle(), CP_BODY_TYPE_DYNAMIC);
    cpBodyInit(new_body.Handle(), body_params.mass, body_params.inertia);
    cpBodySetType(new_body.Handle(), static_cast<cpBodyType>(body_params.type));
    cpBodySetUserData(new_body.Handle(), reinterpret_cast<void*>(id));

    m_impl->space.Add(&new_body);
    m_impl->active_bodies[id] = true;

    return &new_body;
}

void PhysicsSystem::ReleaseBody(uint32_t body_id)
{
    std::vector<cm::ShapeImpl*>& body_shapes = m_impl->bodies_shapes[body_id];
    for(cm::ShapeImpl* shape : body_shapes)
    {
        Impl::ReleaseShapeFunc release_func = m_impl->m_shape_release_funcs[shape];
        (*m_impl.*release_func)(shape->Handle());
        m_impl->m_shape_release_funcs.erase(shape);

        m_impl->space.Remove(shape);
        m_impl->shapes.ReleasePoolData(shape);
    }
    body_shapes.clear();

    cm::BodyImpl& body = m_impl->bodies[body_id];
    body.ClearCollisionHandlers();

    m_impl->space.Remove(&body);
    m_impl->active_bodies[body_id] = false;
}

mono::IShape* PhysicsSystem::AddShape(uint32_t body_id, const CircleComponent& params)
{
    const cpVect offset = cpv(params.offset.x, params.offset.y);

    mono::IBody& body = m_impl->bodies[body_id];
    cpCircleShape* shape_data = m_impl->circle_shape_pool.GetPoolData();
    assert(shape_data != nullptr);

    cpCircleShapeInit(shape_data, body.Handle(), params.radius, offset);

    const float inertia_value = cpMomentForCircle(body.GetMass(), 0.0f, params.radius, offset);

    cm::ShapeImpl* shape_impl = m_impl->shapes.GetPoolData();
    shape_impl->SetShapeHandle((cpShape*)shape_data);
    shape_impl->SetInertia(inertia_value);
    shape_impl->SetSensor(params.is_sensor);
    shape_impl->SetCollisionFilter(params.category, params.mask);

    m_impl->m_shape_release_funcs[shape_impl] = &PhysicsSystem::Impl::ReleaseCircleShape;

    m_impl->bodies_shapes[body_id].push_back(shape_impl);
    m_impl->space.Add(shape_impl);

    return shape_impl;
}

mono::IShape* PhysicsSystem::AddShape(uint32_t body_id, const BoxComponent& box_params)
{
    mono::IBody& body = m_impl->bodies[body_id];
    cpPolyShape* shape_data = m_impl->poly_shape_pool.GetPoolData();
    assert(shape_data != nullptr);

    const float hw = box_params.width / 2.0f;
    const float hh = box_params.height / 2.0f;
    const cpBB box = cpBBOffset(cpBBNew(-hw, -hh, hw, hh), cpv(box_params.offset.x, box_params.offset.y));

    cpBoxShapeInit2(shape_data, body.Handle(), box, 0.0f);

    const float inertia_value = cpMomentForBox(body.GetMass(), box_params.width, box_params.height);

    cm::ShapeImpl* shape_impl = m_impl->shapes.GetPoolData();
    shape_impl->SetShapeHandle((cpShape*)shape_data);
    shape_impl->SetInertia(inertia_value);
    shape_impl->SetSensor(box_params.is_sensor);
    shape_impl->SetCollisionFilter(box_params.category, box_params.mask);

    m_impl->m_shape_release_funcs[shape_impl] = &PhysicsSystem::Impl::ReleasePolyShape;

    m_impl->bodies_shapes[body_id].push_back(shape_impl);
    m_impl->space.Add(shape_impl);

    return shape_impl;
}

mono::IShape* PhysicsSystem::AddShape(uint32_t body_id, const SegmentComponent& segment_params)
{
    const cpVect start = cpv(segment_params.start.x, segment_params.start.y);
    const cpVect end = cpv(segment_params.end.x, segment_params.end.y);

    mono::IBody& body = m_impl->bodies[body_id];
    cpSegmentShape* shape_data = m_impl->segment_shape_pool.GetPoolData();
    assert(shape_data != nullptr);

    cpSegmentShapeInit(shape_data, body.Handle(), start, end, segment_params.radius);

    const float inertia_value = cpMomentForSegment(body.GetMass(), start, end, segment_params.radius);

    cm::ShapeImpl* shape_impl = m_impl->shapes.GetPoolData();
    shape_impl->SetShapeHandle((cpShape*)shape_data);
    shape_impl->SetInertia(inertia_value);
    shape_impl->SetSensor(segment_params.is_sensor);
    shape_impl->SetCollisionFilter(segment_params.category, segment_params.mask);

    m_impl->m_shape_release_funcs[shape_impl] = &PhysicsSystem::Impl::ReleaseSegmentShape;

    m_impl->bodies_shapes[body_id].push_back(shape_impl);
    m_impl->space.Add(shape_impl);

    return shape_impl;
}

mono::IShape* PhysicsSystem::AddShape(uint32_t body_id, const PolyComponent& poly_params)
{
    const auto transform_vector_to_cpv = [&poly_params](const math::Vector& position) {
        return cpv(position.x + poly_params.offset.x, position.y + poly_params.offset.x);
    };

    std::vector<cpVect> vects(poly_params.vertices.size());
    std::transform(poly_params.vertices.begin(), poly_params.vertices.end(), vects.begin(), transform_vector_to_cpv);

    const bool clockwice = math::IsPolygonClockwise(poly_params.vertices);
    if(clockwice)
        std::reverse(vects.begin(), vects.end());

    mono::IBody& body = m_impl->bodies[body_id];
    cpPolyShape* shape_data = m_impl->poly_shape_pool.GetPoolData();
    assert(shape_data != nullptr);

    cpPolyShapeInitRaw(shape_data, body.Handle(), (int)vects.size(), vects.data(), 0.1f);

    const float inertia_value = cpMomentForPoly(body.GetMass(), (int)vects.size(), vects.data(), cpvzero, 1.0f);

    cm::ShapeImpl* shape_impl = m_impl->shapes.GetPoolData();
    shape_impl->SetShapeHandle((cpShape*)shape_data);
    shape_impl->SetInertia(inertia_value);
    shape_impl->SetSensor(poly_params.is_sensor);
    shape_impl->SetCollisionFilter(poly_params.category, poly_params.mask);

    m_impl->m_shape_release_funcs[shape_impl] = &PhysicsSystem::Impl::ReleasePolyShape;

    m_impl->bodies_shapes[body_id].push_back(shape_impl);
    m_impl->space.Add(shape_impl);

    return shape_impl;
}

uint32_t PhysicsSystem::Id() const
{
    return mono::Hash(Name());
}

const char* PhysicsSystem::Name() const
{
    return "physicssystem";
}

uint32_t PhysicsSystem::Capacity() const
{
    return m_impl->bodies.size();
}

void PhysicsSystem::Update(const UpdateContext& update_context)
{
    m_impl->space.Tick(update_context.delta_ms);

    for(size_t index = 0; index < m_impl->bodies.size(); ++index)
    {
        const bool is_active = m_impl->active_bodies[index];
        if(is_active)
        {
            mono::IBody& body = m_impl->bodies[index];
            const math::Vector position = body.GetPosition();
            const float rotation = body.GetAngle();

            math::Matrix& transform = m_transform_system->GetTransform(index);
            transform = math::CreateMatrixFromZRotation(rotation);
            math::Position(transform, position);
        }
    }
}

mono::IBody* PhysicsSystem::GetBody(uint32_t body_id)
{
    return &m_impl->bodies[body_id];
}

uint32_t PhysicsSystem::GetIdFromBody(const mono::IBody* body) const
{
    for(size_t index = 0; index < m_impl->bodies.size(); ++index)
    {
        const bool is_active = m_impl->active_bodies[index];
        if(is_active && &m_impl->bodies[index] == body)
            return index;
    }

    return -1;
}

std::vector<mono::IShape*> PhysicsSystem::GetShapesAttachedToBody(uint32_t body_id) const
{
    std::vector<mono::IShape*> body_shapes;

    for(cm::ShapeImpl* shape_impl : m_impl->bodies_shapes[body_id])
        body_shapes.push_back(shape_impl);

    return body_shapes;
}

mono::IBody* PhysicsSystem::CreateKinematicBody()
{
    cpBody* kinematic_body = cpBodyNewKinematic();
    mono::IBody* body_impl = new cm::BodyImpl(kinematic_body);
    m_impl->space.Add(body_impl);

    return body_impl;
}

void PhysicsSystem::ReleaseKinematicBody(mono::IBody* body)
{
    m_impl->space.Remove(body);
    cpBodyFree(body->Handle());
    delete body;
}

mono::IConstraint* PhysicsSystem::CreateSpring(IBody* first, IBody* second, float rest_length, float stiffness, float damping)
{
    cpDampedSpring* damped_spring_data = m_impl->damped_spring_pool.GetPoolData();
    cpDampedSpringInit(damped_spring_data, first->Handle(), second->Handle(), cpvzero, cpvzero, rest_length, stiffness, damping);

    cm::ConstraintImpl* constraint_impl = m_impl->constraints.GetPoolData();
    constraint_impl->SetHandle((cpConstraint*)damped_spring_data);

    m_impl->m_constraint_release_funcs[constraint_impl] = &PhysicsSystem::Impl::ReleaseSpring;
    m_impl->space.Add(constraint_impl);

    return constraint_impl;
}

void PhysicsSystem::ReleaseConstraint(mono::IConstraint* constraint)
{
    Impl::ReleaseConstraintFunc release_func = m_impl->m_constraint_release_funcs[constraint];
    (*m_impl.*release_func)(constraint->Handle());
    m_impl->m_constraint_release_funcs.erase(constraint);

    m_impl->space.Remove(constraint);
    m_impl->constraints.ReleasePoolData((cm::ConstraintImpl*)constraint);
}

mono::PhysicsSpace* PhysicsSystem::GetSpace()
{
    return &m_impl->space;
}

mono::PhysicsSystemStats PhysicsSystem::GetStats() const
{
    mono::PhysicsSystemStats stats;

    stats.bodies = m_impl->body_pool.Used();
    stats.shapes = m_impl->shapes.Used();
    stats.constraints = m_impl->constraints.Used();
    
    stats.circle_shapes = m_impl->circle_shape_pool.Used();
    stats.segment_shapes = m_impl->segment_shape_pool.Used();
    stats.polygon_shapes = m_impl->poly_shape_pool.Used();
    
    stats.pivot_joints = m_impl->pivot_joint_pool.Used();
    stats.gear_joints = m_impl->gear_joint_pool.Used();
    stats.damped_springs = m_impl->damped_spring_pool.Used();

    return stats;
}
