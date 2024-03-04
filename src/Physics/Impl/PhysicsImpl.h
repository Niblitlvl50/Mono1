
#pragma once

#include "Physics/PhysicsSystem.h"
#include "Physics/PhysicsSpace.h"

#include "Physics/Impl/BodyImpl.h"
#include "Physics/Impl/ShapeImpl.h"
#include "Physics/Impl/ConstraintImpl.h"

#include "Util/ObjectPool.h"

#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_private.h"

#include <unordered_map>

static_assert(static_cast<int>(mono::BodyType::DYNAMIC) == cpBodyType::CP_BODY_TYPE_DYNAMIC);
static_assert(static_cast<int>(mono::BodyType::KINEMATIC) == cpBodyType::CP_BODY_TYPE_KINEMATIC);
static_assert(static_cast<int>(mono::BodyType::STATIC) == cpBodyType::CP_BODY_TYPE_STATIC);

namespace cm
{
    struct PhysicsImpl
    {
        PhysicsImpl(const mono::PhysicsSystemInitParams& init_params, mono::PhysicsSystem* physics_system)
            : body_pool(init_params.n_bodies)
            , circle_shape_pool(init_params.n_circle_shapes)
            , segment_shape_pool(init_params.n_segment_shapes)
            , poly_shape_pool(init_params.n_polygon_shapes)
            , pivot_joint_pool(init_params.n_pivot_joints)
            , slide_joint_pool(init_params.n_slide_joints)
            , gear_joint_pool(init_params.n_gear_joints)
            , damped_spring_pool(init_params.n_damped_springs)
            , shapes(init_params.n_circle_shapes + init_params.n_segment_shapes + init_params.n_polygon_shapes)
            , constraints(init_params.n_pivot_joints + init_params.n_gear_joints + init_params.n_damped_springs)
            , space(physics_system)
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

        void ReleasePivotJoint(cpConstraint* constraint)
        {
            pivot_joint_pool.ReleasePoolData((cpPivotJoint*)constraint);
        }

        void ReleaseSlideJoint(cpConstraint* constraint)
        {
            slide_joint_pool.ReleasePoolData((cpSlideJoint*)constraint);
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
        mono::ObjectPool<cpSlideJoint> slide_joint_pool;
        mono::ObjectPool<cpGearJoint> gear_joint_pool;
        mono::ObjectPool<cpDampedSpring> damped_spring_pool;

        mono::ObjectPool<cm::ShapeImpl> shapes;
        mono::ObjectPool<cm::ConstraintImpl> constraints;

        std::vector<cm::BodyImpl> bodies;
        std::vector<std::vector<cm::ShapeImpl*>> bodies_shapes;
        std::vector<std::vector<cm::ConstraintImpl*>> bodies_constraints;
        std::vector<bool> active_bodies;

        mono::PhysicsSpace space;

        using ReleaseShapeFunc = void (PhysicsImpl::*)(cpShape* handle);
        std::unordered_map<cm::ShapeImpl*, ReleaseShapeFunc> m_shape_release_funcs;

        using ReleaseConstraintFunc = void (PhysicsImpl::*)(cpConstraint* handle);
        std::unordered_map<mono::IConstraint*, ReleaseConstraintFunc> m_constraint_release_funcs;
    };    
}
