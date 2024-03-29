
#pragma once

#include "IBody.h"
#include "Math/Vector.h"
#include "IGameSystem.h"

#include <memory>
#include <vector>
#include <functional>

class PhysicsShapeHelper;

namespace cm
{
    struct PhysicsImpl;
}

namespace mono
{
    class IBody;
    class IShape;
    class IConstraint;
    class PhysicsSpace;

    struct PhysicsSystemInitParams
    {
        uint32_t n_bodies = 100;

        uint32_t n_circle_shapes = 100;
        uint32_t n_segment_shapes = 100;
        uint32_t n_polygon_shapes = 100;

        uint32_t n_pivot_joints = 100;
        uint32_t n_slide_joints = 100;
        uint32_t n_gear_joints = 100;
        uint32_t n_damped_springs = 100;
    };

    struct BodyComponent
    {
        float mass;
        float inertia;
        BodyType type;
    };

    struct CircleComponent
    {
        uint32_t category;
        uint32_t mask;
        float radius;
        math::Vector offset;
        bool is_sensor;
    };

    struct BoxComponent
    {
        uint32_t category;
        uint32_t mask;
        math::Vector size;
        math::Vector offset;
        bool is_sensor;
    };

    struct SegmentComponent
    {
        uint32_t category;
        uint32_t mask;
        math::Vector start;
        math::Vector end;
        float radius;
        bool is_sensor;
    };

    struct PolyComponent
    {
        uint32_t category;
        uint32_t mask;
        std::vector<math::Vector> vertices;
        math::Vector offset;
        bool is_sensor;
    };

    struct PhysicsSystemStats
    {
        uint32_t bodies;
        uint32_t shapes;
        uint32_t constraints;

        uint32_t circle_shapes;
        uint32_t circle_shapes_max;

        uint32_t segment_shapes;
        uint32_t segment_shapes_max;

        uint32_t polygon_shapes;
        uint32_t polygon_shapes_max;

        uint32_t pivot_joints;
        uint32_t pivot_joints_max;

        uint32_t gear_joints;
        uint32_t gear_joints_max;

        uint32_t damped_springs;
        uint32_t damped_springs_max;
    };

    class TransformSystem;

    using ForEachBodyFunc = std::function<void (uint32_t body_id, mono::IBody& body)>;

    class PhysicsSystem : public mono::IGameSystem
    {
    public:

        PhysicsSystem(const PhysicsSystemInitParams& init_params, mono::TransformSystem* transform_system);
        ~PhysicsSystem();

        const char* Name() const override;
        void Update(const UpdateContext& update_context) override;
        void Sync() override;

        mono::IBody* AllocateBody(uint32_t body_id, const BodyComponent& body_params);
        void ReleaseBody(uint32_t body_id);
        bool IsAllocated(uint32_t body_id) const;
        mono::IBody* GetBody(uint32_t body_id);

        mono::IShape* AddShape(uint32_t body_id, const CircleComponent& circle_params);
        mono::IShape* AddShape(uint32_t body_id, const BoxComponent& box_params);
        mono::IShape* AddShape(uint32_t body_id, const SegmentComponent& segment_params);
        mono::IShape* AddShape(uint32_t body_id, const PolyComponent& poly_params);

        std::vector<mono::IShape*> GetShapesAttachedToBody(uint32_t body_id) const;

        mono::IBody* CreateKinematicBody();
        void ReleaseKinematicBody(mono::IBody* body);

        mono::IConstraint* CreateSlideJoint(
            IBody* first, IBody* second, const math::Vector& anchor_first, const math::Vector& anchor_second, float min_length, float max_length);
        mono::IConstraint* CreateSpring(
            IBody* first, IBody* second, const math::Vector& anchor_first, const math::Vector& anchor_second, float rest_length, float stiffness, float damping);
        void ReleaseConstraint(mono::IConstraint* constraint);

        mono::PhysicsSpace* GetSpace() const;
        PhysicsSystemStats GetStats() const;

        void ForEachBody(const ForEachBodyFunc& func);

        static uint32_t GetIdFromBody(const mono::IBody* body);

    private:

        void ReleaseConstraintInternal(mono::IConstraint* constraint);

        friend class ::PhysicsShapeHelper;

        std::unique_ptr<cm::PhysicsImpl> m_impl;
        mono::TransformSystem* m_transform_system;

        std::vector<mono::IBody*> m_deferred_add_bodies;
        std::vector<mono::IShape*> m_deferred_add_shapes;
        std::vector<mono::IConstraint*> m_deferred_add_constraints;
        std::vector<mono::IConstraint*> m_released_constraints;
    };
}
