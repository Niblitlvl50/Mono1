
#pragma once

#include "IBody.h"
#include "Math/Vector.h"
#include "IGameSystem.h"

#include <memory>
#include <vector>

namespace mono
{
    class IBody;
    class IShape;
    class IConstraint;
    class PhysicsSpace;

    struct PhysicsSystemInitParams
    {
        size_t n_bodies = 100;

        size_t n_circle_shapes = 100;
        size_t n_segment_shapes = 100;
        size_t n_polygon_shapes = 100;

        size_t n_pivot_joints = 100;
        size_t n_gear_joints = 100;
        size_t n_damped_springs = 100;
    };

    struct BodyComponent
    {
        float mass;
        float inertia;
        BodyType type;
    };

    enum class ShapeType : int
    {
        CIRCLE,
        BOX,
        SEGMENT,
        POLYGON
    };

    struct CircleComponent
    {
        uint32_t category;
        uint32_t mask;
        float radius;
        math::Vector offset;
    };

    struct BoxComponent
    {
        uint32_t category;
        uint32_t mask;
        float width;
        float height;
        math::Vector offset;
    };

    struct SegmentComponent
    {
        uint32_t category;
        uint32_t mask;
        math::Vector start;
        math::Vector end;
        float radius;
    };

    struct PolyComponent
    {
        uint32_t category;
        uint32_t mask;
        std::vector<math::Vector> vertices;
        math::Vector offset;
    };

    struct PivotComponent
    {
        uint32_t body_a;
        uint32_t body_b;
    };

    struct GearComponent
    {
        uint32_t body_a;
        uint32_t body_b;
        float phase;
        float ratio;
    };

    struct SpringComponent
    {
        uint32_t body_a;
        uint32_t body_b;
        float rest_length;
        float stiffness;
        float damping;
    };

    struct PhysicsSystemStats
    {
        uint32_t bodies;
        uint32_t shapes;
        uint32_t constraints;

        uint32_t circle_shapes;
        uint32_t segment_shapes;
        uint32_t polygon_shapes;

        uint32_t pivot_joints;
        uint32_t gear_joints;
        uint32_t damped_springs;
    };

    class TransformSystem;

    class PhysicsSystem : public mono::IGameSystem
    {
    public:

        PhysicsSystem(const PhysicsSystemInitParams& init_params, mono::TransformSystem* transform_system);
        ~PhysicsSystem();

        uint32_t Id() const override;
        const char* Name() const override;
        uint32_t Capacity() const override;
        void Update(const UpdateContext& update_context) override;

        mono::IBody* AllocateBody(uint32_t body_id, const BodyComponent& body_params);
        void ReleaseBody(uint32_t body_id);
        mono::IBody* GetBody(uint32_t body_id);

        mono::IShape* AddShape(uint32_t body_id, const CircleComponent& circle_params);
        mono::IShape* AddShape(uint32_t body_id, const BoxComponent& box_params);
        mono::IShape* AddShape(uint32_t body_id, const SegmentComponent& segment_params);
        mono::IShape* AddShape(uint32_t body_id, const PolyComponent& poly_params);

        std::vector<mono::IShape*> GetShapesAttachedToBody(uint32_t body_id) const;

        mono::IBody* CreateKinematicBody();
        void ReleaseKinematicBody(mono::IBody* body);

        mono::IConstraint* CreateSpring(IBody* first, IBody* second, float rest_length, float stiffness, float damping);
        void ReleaseConstraint(mono::IConstraint* constraint);

        uint32_t GetIdFromBody(const mono::IBody* body) const;
        mono::PhysicsSpace* GetSpace();
        PhysicsSystemStats GetStats() const;

    private:

        struct Impl;
        std::unique_ptr<Impl> m_impl;
        mono::TransformSystem* m_transform_system;
    };
}