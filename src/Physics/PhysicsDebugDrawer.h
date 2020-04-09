
#pragma once

#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

#include "Rendering/IDrawable.h"
#include "Math/Vector.h"

#include <cstdint>
#include <vector>

namespace mono
{
    class PhysicsSystem;
    class EventHandler;

    enum PhysicsDebugComponents
    {
        NONE = 0,
        DRAW_SHAPES = 1,
        DRAW_CONSTRAINTS = 2,
        DRAW_COLLISION_POINTS = 4,
    };

    enum class PhysicsDebugInteractionFunction
    {
        NEAREST_POINT_QUERY,
        LINE_SEGMENT_QUERY
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
            const bool& enabled,
            const uint32_t& debug_components,
            mono::PhysicsSystem* physics_system,
            mono::EventHandler* event_handler);
        ~PhysicsDebugDrawer();

        void doDraw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        mono::EventResult OnMouseDown(const event::MouseDownEvent& event);
        mono::EventResult OnMouseUp(const event::MouseUpEvent& event);


    private:
        const bool& m_enabled;
        const uint32_t& m_debug_components;

        mono::PhysicsSystem* m_physics_system;
        mono::EventHandler* m_event_handler;

        mono::EventToken<event::MouseDownEvent> m_mouse_down_token;
        mono::EventToken<event::MouseUpEvent> m_mouse_up_token;

        math::Vector m_mouse_down_position;
        math::Vector m_mouse_up_position;

        std::vector<math::Vector> m_found_positions;
        mutable uint32_t m_click_timestamp;
    };
}
