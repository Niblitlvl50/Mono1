
#pragma once

#include "MonoFwd.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

#include "Rendering/IDrawable.h"
#include "Math/Vector.h"

#include <cstdint>
#include <vector>

namespace mono
{
    enum PhysicsDebugComponents
    {
        NONE = 0,
        DRAW_SHAPES = 1,
        DRAW_CONSTRAINTS = 2,
        DRAW_COLLISION_POINTS = 4,
        DRAW_BODY_FORCES = 8,
        DRAW_SLEEP_STATE = 16,
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
        PhysicsDebugComponents::DRAW_BODY_FORCES,
        PhysicsDebugComponents::DRAW_SLEEP_STATE,
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
        case DRAW_BODY_FORCES:
            return "Forces";
        case DRAW_SLEEP_STATE:
            return "Sleep State";
        }

        return "Unknown";
    }

    class PhysicsDebugDrawer : public mono::IDrawable
    {
    public:

        PhysicsDebugDrawer(
            const bool& enabled_drawing,
            const bool& enabled_interaction,
            const bool& enabled_body_introspection,
            const uint32_t& debug_components,
            mono::PhysicsSystem* physics_system,
            mono::EventHandler* event_handler);
        ~PhysicsDebugDrawer();

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        void DrawBodyIntrospection(mono::IRenderer& renderer) const;
        void DrawPhysics(const mono::IRenderer& renderer) const;
        void DrawForces(mono::IRenderer& renderer) const;
        void DrawSleepState(mono::IRenderer& renderer) const;

        mono::EventResult OnMouseDown(const event::MouseDownEvent& event);
        mono::EventResult OnMouseUp(const event::MouseUpEvent& event);
        mono::EventResult OnMouseMove(const event::MouseMotionEvent& event);

        mono::EventResult OnKeyDown(const event::KeyDownEvent& event);
        mono::EventResult OnKeyUp(const event::KeyUpEvent& event);

    private:
        const bool& m_enabled_drawing;
        const bool& m_enabled_interaction;
        const bool& m_enabled_body_introspection;
        const uint32_t& m_debug_components;

        mono::PhysicsSystem* m_physics_system;
        mono::EventHandler* m_event_handler;

        mono::EventToken<event::MouseDownEvent> m_mouse_down_token;
        mono::EventToken<event::MouseUpEvent> m_mouse_up_token;
        mono::EventToken<event::MouseMotionEvent> m_mouse_move_token;
        mono::EventToken<event::KeyDownEvent> m_key_down_token;
        mono::EventToken<event::KeyUpEvent> m_key_up_token;

        math::Vector m_mouse_down_position;
        math::Vector m_mouse_up_position;
        math::Vector m_mouse_position;

        std::vector<math::Vector> m_found_positions;
        mutable uint32_t m_click_timestamp;

        bool m_mouse_down;
        bool m_shift_down;

        mutable int m_body_id;
    };
}
