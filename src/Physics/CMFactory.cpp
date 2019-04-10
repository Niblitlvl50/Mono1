
#include "CMFactory.h"
#include "Impl/BodyImpl.h"
#include "Impl/ShapeImpl.h"
#include "Impl/ConstraintImpl.h"

#include "Math/Vector.h"
#include "Math/MathFunctions.h"

#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_private.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdio>

namespace
{
    template <typename T>
    struct ObjectPool
    {
        T* data;
        size_t size;
        std::vector<size_t> free_indices;
    };

    template <typename T>
    ObjectPool<T> MakePool(size_t pool_size)
    {
        ObjectPool<T> pool_data;
        pool_data.data = (T*)cpcalloc(pool_size, sizeof(T));
        pool_data.size = pool_size;
        pool_data.free_indices.resize(pool_size);
        
        std::iota(pool_data.free_indices.begin(), pool_data.free_indices.end(), 0);
        std::reverse(pool_data.free_indices.begin(), pool_data.free_indices.end());

        return pool_data;
    }

    template <typename T>
    T* GetPoolData(ObjectPool<T>& pool_object)
    {
        if(pool_object.free_indices.empty())
            return nullptr;

        const size_t shape_index = pool_object.free_indices.back();
        pool_object.free_indices.pop_back();
        return &pool_object.data[shape_index];
    }

    template <typename T>
    void ReleasePoolData(ObjectPool<T>& pool_object, T* data)
    {
        size_t index = 0;
        while(index < pool_object.size)
        {
            T* other_data = &pool_object.data[index];
            if(data == other_data)
                break;

            ++index;
        }

        pool_object.free_indices.push_back(index);
    }

    ObjectPool<cpBody> s_body_pool;
    ObjectPool<cpCircleShape> s_circle_shape_pool;
    ObjectPool<cpSegmentShape> s_segment_shape_pool;
    ObjectPool<cpPolyShape> s_poly_shape_pool;
    ObjectPool<cpPivotJoint> s_pivot_joint_pool;
    ObjectPool<cpGearJoint> s_gear_joint_pool;
    ObjectPool<cpDampedSpring> s_damped_spring_pool;

    cpBody* InitBody(float mass, float inertia, cpBodyType body_type)
    {
        cpBody* body = GetPoolData(s_body_pool);
        cpBodySetType(body, CP_BODY_TYPE_DYNAMIC);
        cpBodyInit(body, mass, inertia);
        cpBodySetType(body, body_type);

        return body;
    }

    void FreeBody(cm::BodyImpl* body_impl)
    {
        cpBody* body = body_impl->Handle();
        ReleasePoolData(s_body_pool, body);
    }

    void FreeCircleShape(cm::ShapeImpl* shape_impl)
    {
        cpShape* shape = shape_impl->Handle();
        ReleasePoolData(s_circle_shape_pool, (cpCircleShape*)shape);
    }

    void FreePolyShape(cm::ShapeImpl* shape_impl)
    {
        cpShape* shape = shape_impl->Handle();
        ReleasePoolData(s_poly_shape_pool, (cpPolyShape*)shape);
    }

    void FreeSegmentShape(cm::ShapeImpl* shape_impl)
    {
        cpShape* shape = shape_impl->Handle();
        ReleasePoolData(s_segment_shape_pool, (cpSegmentShape*)shape);
    }

    void FreePivotJoint(cm::ConstraintImpl* constraint_impl)
    {
        cpConstraint* constraint = constraint_impl->Handle();
        ReleasePoolData(s_pivot_joint_pool, (cpPivotJoint*)constraint);
    }

    void FreeGearJoint(cm::ConstraintImpl* constraint_impl)
    {
        cpConstraint* constraint = constraint_impl->Handle();
        ReleasePoolData(s_gear_joint_pool, (cpGearJoint*)constraint);
    }

    void FreeDampedSpring(cm::ConstraintImpl* constraint_impl)
    {
        cpConstraint* constraint = constraint_impl->Handle();
        ReleasePoolData(s_damped_spring_pool, (cpDampedSpring*)constraint);
    }
}

void mono::PhysicsFactory::Init(const PhysicsInitParams& init_params)
{
    s_body_pool = MakePool<cpBody>(init_params.n_bodies);

    s_circle_shape_pool = MakePool<cpCircleShape>(init_params.n_circle_shapes);
    s_segment_shape_pool = MakePool<cpSegmentShape>(init_params.n_segment_shapes);
    s_poly_shape_pool = MakePool<cpPolyShape>(init_params.n_polygon_shapes);

    s_pivot_joint_pool = MakePool<cpPivotJoint>(init_params.n_pivot_joints);
    s_gear_joint_pool = MakePool<cpGearJoint>(init_params.n_gear_joints);
    s_damped_spring_pool = MakePool<cpDampedSpring>(init_params.n_damped_springs);

    std::printf(
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

void mono::PhysicsFactory::Shutdown()
{    
    for(size_t index = 0; index < s_body_pool.size; ++index)
        cpBodyDestroy(&s_body_pool.data[index]);
    cpfree(s_body_pool.data);

    for(size_t index = 0; index < s_circle_shape_pool.size; ++index)
        cpShapeDestroy((cpShape*)&s_circle_shape_pool.data[index]);
    cpfree(s_circle_shape_pool.data);

    for(size_t index = 0; index < s_segment_shape_pool.size; ++index)
        cpShapeDestroy((cpShape*)&s_segment_shape_pool.data[index]);
    cpfree(s_segment_shape_pool.data);

    for(size_t index = 0; index < s_poly_shape_pool.size; ++index)
        cpShapeDestroy((cpShape*)&s_poly_shape_pool.data[index]);
    cpfree(s_poly_shape_pool.data);

    for(size_t index = 0; index < s_pivot_joint_pool.size; ++index)
        cpConstraintDestroy((cpConstraint*)&s_pivot_joint_pool.data[index]);    
    cpfree(s_pivot_joint_pool.data);

    for(size_t index = 0; index < s_gear_joint_pool.size; ++index)
        cpConstraintDestroy((cpConstraint*)&s_gear_joint_pool.data[index]);    
    cpfree(s_gear_joint_pool.data);

    for(size_t index = 0; index < s_damped_spring_pool.size; ++index)
        cpConstraintDestroy((cpConstraint*)&s_damped_spring_pool.data[index]);    
    cpfree(s_damped_spring_pool.data);
}

mono::IBodyPtr mono::PhysicsFactory::CreateStaticBody()
{
    cpBody* body = InitBody(0.0f, 0.0f, CP_BODY_TYPE_STATIC);
    return std::shared_ptr<cm::BodyImpl>(new cm::BodyImpl(body), FreeBody);
}

mono::IBodyPtr mono::PhysicsFactory::CreateKinematicBody()
{
    cpBody* body = InitBody(0.0f, 0.0f, CP_BODY_TYPE_KINEMATIC);
    return std::shared_ptr<cm::BodyImpl>(new cm::BodyImpl(body), FreeBody);
}

mono::IBodyPtr mono::PhysicsFactory::CreateBody(float mass, float inertia)
{
    cpBody* body = InitBody(mass, inertia, CP_BODY_TYPE_DYNAMIC);
    return std::shared_ptr<cm::BodyImpl>(new cm::BodyImpl(body), FreeBody);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, float radius, const math::Vector& offset)
{
    cpCircleShape* shape_data = GetPoolData(s_circle_shape_pool);
    cpCircleShapeInit(shape_data, body->Handle(), radius, cpv(offset.x, offset.y));

    const float inertia_value = cpMomentForCircle(body->GetMass(), 0.0f, radius, cpv(offset.x, offset.y));
    return std::shared_ptr<cm::ShapeImpl>(new cm::ShapeImpl((cpShape*)shape_data, inertia_value), FreeCircleShape);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, float width, float height)
{
    cpPolyShape* shape_data = GetPoolData(s_poly_shape_pool);
    cpBoxShapeInit(shape_data, body->Handle(), width, height, 0.0f);

    const float inertia_value = cpMomentForBox(body->GetMass(), width, height);
    return std::shared_ptr<cm::ShapeImpl>(new cm::ShapeImpl((cpShape*)shape_data, inertia_value), FreePolyShape);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, const math::Vector& first, const math::Vector& second, float radius)
{
    cpSegmentShape* shape_data = GetPoolData(s_segment_shape_pool);
    cpSegmentShapeInit(shape_data, body->Handle(), cpv(first.x, first.y), cpv(second.x, second.y), radius);

    const float inertia_value = cpMomentForSegment(body->GetMass(), cpv(first.x, first.y), cpv(second.x, second.y), radius);
    return std::shared_ptr<cm::ShapeImpl>(new cm::ShapeImpl((cpShape*)shape_data, inertia_value), FreeSegmentShape);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, const std::vector<math::Vector>& vertices, const math::Vector& offset)
{
    const auto transform_vector_to_cpv = [](const math::Vector& position) {
        return cpv(position.x, position.y);
    };
    
    std::vector<cpVect> vects(vertices.size());
    std::transform(vertices.begin(), vertices.end(), vects.begin(), transform_vector_to_cpv);

    const bool clockwice = math::IsPolygonClockwise(vertices);
    if(clockwice)
        std::reverse(vects.begin(), vects.end());

    cpPolyShape* shape_data = GetPoolData(s_poly_shape_pool);
	cpPolyShapeInitRaw(shape_data, body->Handle(), int(vects.size()), vects.data(), 0.1f);

    const float inertia_value = cpMomentForPoly(body->GetMass(), int(vects.size()), vects.data(), cpv(offset.x, offset.y), 1.0f);
    return std::shared_ptr<cm::ShapeImpl>(new cm::ShapeImpl((cpShape*)shape_data, inertia_value), FreePolyShape);
}

mono::IConstraintPtr mono::PhysicsFactory::CreatePivot(const mono::IBodyPtr& a, const mono::IBodyPtr& b)
{
    const cpVect pivot = cpvzero;
    const cpVect anchorA = (a ? cpBodyWorldToLocal(a->Handle(), pivot) : pivot);
	const cpVect anchorB = (b ? cpBodyWorldToLocal(b->Handle(), pivot) : pivot);

    cpPivotJoint* pivot_data = GetPoolData(s_pivot_joint_pool);
    cpPivotJointInit(pivot_data, a->Handle(), b->Handle(), anchorA, anchorB);

    return std::shared_ptr<cm::ConstraintImpl>(new cm::ConstraintImpl((cpConstraint*)pivot_data), FreePivotJoint);
}

mono::IConstraintPtr mono::PhysicsFactory::CreateGear(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float phase, float ratio)
{
    cpGearJoint* gear_joint_data = GetPoolData(s_gear_joint_pool);
    cpGearJointInit(gear_joint_data, a->Handle(), b->Handle(), phase, ratio);
    return std::shared_ptr<cm::ConstraintImpl>(new cm::ConstraintImpl((cpConstraint*)gear_joint_data), FreeGearJoint);
}

mono::IConstraintPtr mono::PhysicsFactory::CreateSpring(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float rest_length, float stiffness, float damping)
{
    cpDampedSpring* damped_spring_data = GetPoolData(s_damped_spring_pool);
    cpDampedSpringInit(damped_spring_data, a->Handle(), b->Handle(), cpvzero, cpvzero, rest_length, stiffness, damping);
    return std::shared_ptr<cm::ConstraintImpl>(new cm::ConstraintImpl((cpConstraint*)damped_spring_data), FreeDampedSpring);
}

mono::IConstraintPtr mono::PhysicsFactory::CreateSpring(const mono::IBody* a, const mono::IBody* b, float rest_length, float stiffness, float damping)
{
    cpDampedSpring* damped_spring_data = GetPoolData(s_damped_spring_pool);
    cpDampedSpringInit(damped_spring_data, a->Handle(), b->Handle(), cpvzero, cpvzero, rest_length, stiffness, damping);
    return std::shared_ptr<cm::ConstraintImpl>(new cm::ConstraintImpl((cpConstraint*)damped_spring_data), FreeDampedSpring);
}
