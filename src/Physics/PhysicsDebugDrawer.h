
#pragma once

#include "Rendering/IDrawable.h"
#include <cstdint>

namespace mono
{
    class PhysicsSystem;

    enum PhysicsDebugComponents
    {
        NONE = 0,
        DRAW_SHAPES = 1,
        DRAW_CONSTRAINTS = 2,
        DRAW_COLLISION_POINTS = 4,
    };

    constexpr uint32_t all_physics_debug_component[] = {
        PhysicsDebugComponents::DRAW_SHAPES,
        PhysicsDebugComponents::DRAW_CONSTRAINTS,
        PhysicsDebugComponents::DRAW_COLLISION_POINTS,
    };

    inline const char* PhsicsDebugComponentToString(uint32_t debug_component)
    {
        switch(debug_component)
        {
        case NONE:
            return "None";
        case DRAW_SHAPES:
            return "Shapes";
        case DRAW_CONSTRAINTS:
            return "Constraints";
        case DRAW_COLLISION_POINTS:
            return "Collisions";
        }

        return "Unknown";
    }

    class PhysicsDebugDrawer : public mono::IDrawable
    {
    public:

        PhysicsDebugDrawer(
            const bool& enabled, const uint32_t& debug_components, mono::PhysicsSystem* physics_system);
        
        void doDraw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:
        const bool& m_enabled;
        const uint32_t& m_debug_components;
        mono::PhysicsSystem* m_physics_system;
    };
}
