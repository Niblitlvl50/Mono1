
#include "PhysicsSystem.h"
#include "TransformSystem/TransformSystem.h"
#include "PhysicsSpace.h"
#include "System/Hash.h"

#include "Impl/PhysicsImpl.h"
#include "Impl/BodyImpl.h"
#include "Impl/ShapeImpl.h"
#include "Impl/ConstraintImpl.h"

#include "Math/MathFunctions.h"
#include "Util/Algorithm.h"
#include "Util/ObjectPool.h"
#include "System/System.h"
#include "System/Debug.h"

#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_private.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdio>
#include <unordered_map>

using namespace mono;

class PhysicsShapeHelper
{
public:

    static mono::IShape* FinalizeAddShape(
        cm::PhysicsImpl* impl,
        mono::PhysicsSystem* physics_system,
        mono::IBody* body,
        cpShape* shape_handle,
        cm::PhysicsImpl::ReleaseShapeFunc release_func,
        float inertia_value,
        bool is_sensor,
        uint32_t category,
        uint32_t mask)
    {
        cm::ShapeImpl* shape_impl = impl->shapes.GetPoolData();
        shape_impl->SetShapeHandle(shape_handle);
        shape_impl->SetInertia(inertia_value);
        shape_impl->SetSensor(is_sensor);
        shape_impl->SetCollisionFilter(category, mask);

        impl->m_shape_release_funcs[shape_impl] = release_func;

        std::vector<cm::ShapeImpl*>& shapes_for_body = impl->bodies_shapes[body->GetId()];
        shapes_for_body.push_back(shape_impl);

        if(body->AutoCalculateMoment())
        {
            float inertia = 0.0f;
            for(const cm::ShapeImpl* shape : shapes_for_body)
                inertia += shape->GetInertiaValue();
            body->SetMoment(inertia);
        }

        physics_system->m_deferred_add_shapes.push_back(shape_impl);
        return shape_impl;
    }
};


PhysicsSystem::PhysicsSystem(const PhysicsSystemInitParams& init_params, mono::TransformSystem* transform_system)
    : m_impl(std::make_unique<cm::PhysicsImpl>(init_params, this))
    , m_transform_system(transform_system)
{
    m_impl->bodies.reserve(init_params.n_bodies);
    for(uint32_t index = 0; index < init_params.n_bodies; ++index)
    {
        cpBody* body = m_impl->body_pool.GetPoolData();
        m_impl->bodies.emplace_back(index, body);

        cm::BodyImpl& body_impl = m_impl->bodies[index];
        cpBodySetUserData(body, &body_impl);
    }

    m_impl->bodies_shapes.resize(init_params.n_bodies);
    m_impl->bodies_constraints.resize(init_params.n_bodies);
    m_impl->active_bodies.resize(init_params.n_bodies, false);

    System::Log(
        "Physics\n"
        "\tchipmunk version: %s\n"
        "\tbodies: %u\n"
        "\tshapes: circle %u, segment %u, polygon %u\n"
        "\tjoints: pivot %u, gear %u\n"
        "\tsprings: damped %u",
        cpVersionString,
        init_params.n_bodies,
        init_params.n_circle_shapes, init_params.n_segment_shapes, init_params.n_polygon_shapes,
        init_params.n_pivot_joints, init_params.n_gear_joints,
        init_params.n_damped_springs);
}

PhysicsSystem::~PhysicsSystem()
{
    for(uint32_t index = 0; index < m_impl->body_pool.Size(); ++index)
        cpBodyDestroy(&m_impl->body_pool.Data()[index]);

    for(uint32_t index = 0; index < m_impl->circle_shape_pool.Size(); ++index)
        cpShapeDestroy((cpShape*)&m_impl->circle_shape_pool.Data()[index]);

    for(uint32_t index = 0; index < m_impl->segment_shape_pool.Size(); ++index)
        cpShapeDestroy((cpShape*)&m_impl->segment_shape_pool.Data()[index]);

    for(uint32_t index = 0; index < m_impl->poly_shape_pool.Size(); ++index)
        cpShapeDestroy((cpShape*)&m_impl->poly_shape_pool.Data()[index]);

    for(uint32_t index = 0; index < m_impl->pivot_joint_pool.Size(); ++index)
        cpConstraintDestroy((cpConstraint*)&m_impl->pivot_joint_pool.Data()[index]);

    for(uint32_t index = 0; index < m_impl->slide_joint_pool.Size(); ++index)
        cpConstraintDestroy((cpConstraint*)&m_impl->slide_joint_pool.Data()[index]);

    for(uint32_t index = 0; index < m_impl->gear_joint_pool.Size(); ++index)
        cpConstraintDestroy((cpConstraint*)&m_impl->gear_joint_pool.Data()[index]);

    for(uint32_t index = 0; index < m_impl->damped_spring_pool.Size(); ++index)
        cpConstraintDestroy((cpConstraint*)&m_impl->damped_spring_pool.Data()[index]);
}

mono::IBody* PhysicsSystem::AllocateBody(uint32_t id, const BodyComponent& body_params)
{
    cm::BodyImpl& new_body = m_impl->bodies[id];

    cpBodyInit(new_body.Handle(), body_params.mass, body_params.inertia);
    cpBodySetType(new_body.Handle(), static_cast<cpBodyType>(body_params.type));
    cpBodySetUserData(new_body.Handle(), &new_body);

    m_deferred_add_bodies.push_back(&new_body);
    return &new_body;
}

void PhysicsSystem::ReleaseBody(uint32_t body_id)
{
    cm::BodyImpl& body = m_impl->bodies[body_id];

    // Dynamic is the default type, and needs to be reset to that so the cpBodyInit works
    // as expected when this body is reused again. 
    cpBodySetType(body.Handle(), CP_BODY_TYPE_DYNAMIC);
    cpBodySetMass(body.Handle(), 1.0f);

    body.ClearCollisionHandlers();
    body.ResetForces();

    std::vector<cm::ShapeImpl*>& body_shapes = m_impl->bodies_shapes[body_id];
    for(cm::ShapeImpl* shape : body_shapes)
    {
        m_impl->space.Remove(shape);

        cm::PhysicsImpl::ReleaseShapeFunc release_func = m_impl->m_shape_release_funcs[shape];
        (*m_impl.*release_func)(shape->Handle());
        m_impl->m_shape_release_funcs.erase(shape);

        m_impl->shapes.ReleasePoolData(shape);
    }

    const std::vector<cm::ConstraintImpl*>& constraints_for_body = m_impl->bodies_constraints[body_id];
    (void)constraints_for_body;
    MONO_ASSERT(constraints_for_body.empty());

    body_shapes.clear();

    m_impl->space.Remove(&body);
    m_impl->active_bodies[body_id] = false;
}

bool PhysicsSystem::IsAllocated(uint32_t body_id) const
{
    if(body_id >= m_impl->body_pool.Size())
        return false;

    return m_impl->active_bodies[body_id];
}

mono::IShape* PhysicsSystem::AddShape(uint32_t body_id, const CircleComponent& params)
{
    mono::IBody& body = m_impl->bodies[body_id];

    cpCircleShape* shape_data = m_impl->circle_shape_pool.GetPoolData();
    MONO_ASSERT(shape_data != nullptr);

    const cpVect offset = cpv(params.offset.x, params.offset.y);
    cpCircleShapeInit(shape_data, body.Handle(), params.radius, offset);

    const float inertia_value = cpMomentForCircle(body.GetMass(), 0.0f, params.radius, offset);
    return ::PhysicsShapeHelper::FinalizeAddShape(
        m_impl.get(), this, &body, (cpShape*)shape_data, &cm::PhysicsImpl::ReleaseCircleShape, inertia_value, params.is_sensor, params.category, params.mask);
}

mono::IShape* PhysicsSystem::AddShape(uint32_t body_id, const BoxComponent& params)
{
    mono::IBody& body = m_impl->bodies[body_id];
    cpPolyShape* shape_data = m_impl->poly_shape_pool.GetPoolData();
    MONO_ASSERT(shape_data != nullptr);

    const float hw = params.size.x / 2.0f;
    const float hh = params.size.y / 2.0f;
    const cpBB box = cpBBOffset(cpBBNew(-hw, -hh, hw, hh), cpv(params.offset.x, params.offset.y));

    cpBoxShapeInit2(shape_data, body.Handle(), box, 0.0f);

    const float inertia_value = cpMomentForBox(body.GetMass(), params.size.x, params.size.y);
    return ::PhysicsShapeHelper::FinalizeAddShape(
        m_impl.get(), this, &body, (cpShape*)shape_data, &cm::PhysicsImpl::ReleasePolyShape, inertia_value, params.is_sensor, params.category, params.mask);
}

mono::IShape* PhysicsSystem::AddShape(uint32_t body_id, const SegmentComponent& params)
{
    const cpVect start = cpv(params.start.x, params.start.y);
    const cpVect end = cpv(params.end.x, params.end.y);

    mono::IBody& body = m_impl->bodies[body_id];
    cpSegmentShape* shape_data = m_impl->segment_shape_pool.GetPoolData();
    MONO_ASSERT(shape_data != nullptr);

    cpSegmentShapeInit(shape_data, body.Handle(), start, end, params.radius);

    const float inertia_value = cpMomentForSegment(body.GetMass(), start, end, params.radius);
    return ::PhysicsShapeHelper::FinalizeAddShape(
        m_impl.get(), this, &body, (cpShape*)shape_data, &cm::PhysicsImpl::ReleaseSegmentShape, inertia_value, params.is_sensor, params.category, params.mask);
}

mono::IShape* PhysicsSystem::AddShape(uint32_t body_id, const PolyComponent& params)
{
    const auto transform_vector_to_cpv = [&params](const math::Vector& position) {
        return cpv(position.x + params.offset.x, position.y + params.offset.x);
    };

    std::vector<cpVect> vects(params.vertices.size());
    std::transform(params.vertices.begin(), params.vertices.end(), vects.begin(), transform_vector_to_cpv);

    const bool clockwice = math::IsPolygonClockwise(params.vertices);
    if(clockwice)
        std::reverse(vects.begin(), vects.end());

    mono::IBody& body = m_impl->bodies[body_id];
    cpPolyShape* shape_data = m_impl->poly_shape_pool.GetPoolData();
    MONO_ASSERT(shape_data != nullptr);

    cpPolyShapeInitRaw(shape_data, body.Handle(), (int)vects.size(), vects.data(), 0.1f);

    const float inertia_value = cpMomentForPoly(body.GetMass(), (int)vects.size(), vects.data(), cpvzero, 1.0f);
    return ::PhysicsShapeHelper::FinalizeAddShape(
        m_impl.get(), this, &body, (cpShape*)shape_data, &cm::PhysicsImpl::ReleasePolyShape, inertia_value, params.is_sensor, params.category, params.mask);
}

const char* PhysicsSystem::Name() const
{
    return "physicssystem";
}

void PhysicsSystem::Update(const UpdateContext& update_context)
{
    if(update_context.paused)
        return;

    for(uint32_t index = 0; index < m_impl->active_bodies.size(); ++index)
    {
        const bool is_active = m_impl->active_bodies[index];
        if(!is_active)
            continue;

        const mono::TransformState state = m_transform_system->GetTransformState(index);
        if(state == TransformState::CLIENT)
        {
            const math::Matrix& transform = m_transform_system->GetTransform(index);
            mono::IBody& body = m_impl->bodies[index];
            body.SetPosition(math::GetPosition(transform));
            body.SetAngle(math::GetZRotation(transform));

            if(body.GetType() == BodyType::STATIC)
                GetSpace()->UpdateBodyShapes(&body);
        }
    }

    m_impl->space.Tick(update_context.delta_s);

    for(uint32_t index = 0; index < m_impl->active_bodies.size(); ++index)
    {
        const bool is_active = m_impl->active_bodies[index];
        if(!is_active)
            continue;

        const mono::IBody& body = m_impl->bodies[index];

        math::Matrix& transform = m_transform_system->GetTransform(index);
        transform = math::CreateMatrixWithPositionRotation(body.GetPosition(), body.GetAngle());
        m_transform_system->SetTransformState(index, TransformState::PHYSICS);
    }
}

void PhysicsSystem::Sync()
{
    for(mono::IBody* body : m_deferred_add_bodies)
    {
        m_impl->active_bodies[body->GetId()] = true;
        m_impl->space.Add(body);
    }
    m_deferred_add_bodies.clear();

    for(mono::IShape* shape : m_deferred_add_shapes)
        m_impl->space.Add(shape);
    m_deferred_add_shapes.clear();

    for(mono::IConstraint* constraint : m_deferred_add_constraints)
        m_impl->space.Add(constraint);
    m_deferred_add_constraints.clear();

    for(mono::IConstraint* constraint : m_released_constraints)
        ReleaseConstraintInternal(constraint);
    m_released_constraints.clear();
}

mono::IBody* PhysicsSystem::GetBody(uint32_t body_id)
{
    return &m_impl->bodies[body_id];
}

void PhysicsSystem::ForEachBody(const ForEachBodyFunc& func)
{
    for(uint32_t index = 0; index < m_impl->active_bodies.size(); ++index)
    {
        const bool is_enabled = m_impl->active_bodies[index];
        if(is_enabled)
            func(index, m_impl->bodies[index]);
    }
}

uint32_t PhysicsSystem::GetIdFromBody(const mono::IBody* body)
{
    if(body)
        return body->GetId();

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
    mono::IBody* body_impl = new cm::BodyImpl(-1, kinematic_body);
    m_impl->space.Add(body_impl);

    return body_impl;
}

void PhysicsSystem::ReleaseKinematicBody(mono::IBody* body)
{
    m_impl->space.Remove(body);
    cpBodyFree(body->Handle());
    delete body;
}

mono::IConstraint* PhysicsSystem::CreateSlideJoint(
    IBody* first, IBody* second, const math::Vector& anchor_first, const math::Vector& anchor_second, float min_length, float max_length)
{
    cpSlideJoint* slide_joint_data = m_impl->slide_joint_pool.GetPoolData();
    cpSlideJointInit(
        slide_joint_data,
        first->Handle(),
        second->Handle(),
        cpv(anchor_first.x, anchor_first.y),
        cpv(anchor_second.x, anchor_second.y),
        min_length,
        max_length);

    cm::ConstraintImpl* constraint_impl = m_impl->constraints.GetPoolData();
    constraint_impl->Init(this);
    constraint_impl->SetHandle((cpConstraint*)slide_joint_data);

    m_impl->m_constraint_release_funcs[constraint_impl] = &cm::PhysicsImpl::ReleaseSlideJoint;

    std::vector<cm::ConstraintImpl*>& constraints_for_body_1 = m_impl->bodies_constraints[first->GetId()];
    constraints_for_body_1.push_back(constraint_impl);

    std::vector<cm::ConstraintImpl*>& constraints_for_body_2 = m_impl->bodies_constraints[second->GetId()];
    constraints_for_body_2.push_back(constraint_impl);

    m_deferred_add_constraints.push_back(constraint_impl);
    return constraint_impl;
}

mono::IConstraint* PhysicsSystem::CreateSpring(
    IBody* first, IBody* second, const math::Vector& anchor_first, const math::Vector& anchor_second, float rest_length, float stiffness, float damping)
{
    cpDampedSpring* damped_spring_data = m_impl->damped_spring_pool.GetPoolData();
    cpDampedSpringInit(
        damped_spring_data, first->Handle(), second->Handle(), cpv(anchor_first.x, anchor_first.y), cpv(anchor_second.x, anchor_second.y), rest_length, stiffness, damping);

    cm::ConstraintImpl* constraint_impl = m_impl->constraints.GetPoolData();
    constraint_impl->Init(this);
    constraint_impl->SetHandle((cpConstraint*)damped_spring_data);

    m_impl->m_constraint_release_funcs[constraint_impl] = &cm::PhysicsImpl::ReleaseSpring;

    std::vector<cm::ConstraintImpl*>& constraints_for_body_1 = m_impl->bodies_constraints[first->GetId()];
    constraints_for_body_1.push_back(constraint_impl);

    std::vector<cm::ConstraintImpl*>& constraints_for_body_2 = m_impl->bodies_constraints[second->GetId()];
    constraints_for_body_2.push_back(constraint_impl);

    m_deferred_add_constraints.push_back(constraint_impl);
    return constraint_impl;
}

void PhysicsSystem::ReleaseConstraint(mono::IConstraint* constraint)
{
    m_released_constraints.push_back(constraint);
}

void PhysicsSystem::ReleaseConstraintInternal(mono::IConstraint* constraint)
{
    cm::ConstraintImpl* constraint_impl = (cm::ConstraintImpl*)constraint;

    const mono::ConstraintBodyPair body_pair = constraint->GetBodies();
    const uint32_t first_body_id = body_pair.first->GetId();
    const uint32_t second_body_id = body_pair.second->GetId();

    std::vector<cm::ConstraintImpl*>& constraints_for_body_1 = m_impl->bodies_constraints[first_body_id];
    mono::remove(constraints_for_body_1, constraint_impl);

    std::vector<cm::ConstraintImpl*>& constraints_for_body_2 = m_impl->bodies_constraints[second_body_id];
    mono::remove(constraints_for_body_2, constraint_impl);

    m_impl->space.Remove(constraint);

    cm::PhysicsImpl::ReleaseConstraintFunc release_func = m_impl->m_constraint_release_funcs[constraint];
    (*m_impl.*release_func)(constraint->Handle());
    m_impl->m_constraint_release_funcs.erase(constraint);

    m_impl->constraints.ReleasePoolData(constraint_impl);
}

mono::PhysicsSpace* PhysicsSystem::GetSpace() const
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
    stats.circle_shapes_max = m_impl->circle_shape_pool.Size();
    stats.segment_shapes = m_impl->segment_shape_pool.Used();
    stats.segment_shapes_max = m_impl->segment_shape_pool.Size();
    stats.polygon_shapes = m_impl->poly_shape_pool.Used();
    stats.polygon_shapes_max = m_impl->poly_shape_pool.Size();
    
    stats.pivot_joints = m_impl->pivot_joint_pool.Used();
    stats.pivot_joints_max = m_impl->pivot_joint_pool.Size();
    stats.gear_joints = m_impl->gear_joint_pool.Used();
    stats.gear_joints_max = m_impl->gear_joint_pool.Size();
    stats.damped_springs = m_impl->damped_spring_pool.Used();
    stats.damped_springs_max = m_impl->damped_spring_pool.Size();

    return stats;
}
