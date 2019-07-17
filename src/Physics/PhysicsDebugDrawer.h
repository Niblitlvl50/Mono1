
#pragma once

#include "Rendering/IDrawable.h"

namespace mono
{
    class PhysicsSystem;

    class PhysicsDebugDrawer : public mono::IDrawable
    {
    public:

        PhysicsDebugDrawer(mono::PhysicsSystem* physics_system);

        void doDraw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:
        mono::PhysicsSystem* m_physics_system;
        bool m_draw_shapes;
        bool m_draw_constraints;
        bool m_draw_collisions;
    };
}