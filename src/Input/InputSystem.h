
#pragma once

#include "MonoFwd.h"
#include "IGameSystem.h"

#include "EventHandler/EventToken.h"
#include "Events/MouseEvent.h"
#include "Events/ControllerEvent.h"
#include "Events/KeyEvent.h"

#include <vector>

namespace mono
{
    enum class InputResult
    {
        Handled,
        Pass
    };

    class IMouseInput
    {
    public:

        virtual ~IMouseInput() = default;
        virtual mono::InputResult Move(const event::MouseMotionEvent& event) { return mono::InputResult::Pass; };
        virtual mono::InputResult ButtonDown(const event::MouseDownEvent& event) { return mono::InputResult::Pass; };
        virtual mono::InputResult ButtonUp(const event::MouseUpEvent& event) { return mono::InputResult::Pass; };
        virtual mono::InputResult Wheel(const event::MouseWheelEvent& event) { return mono::InputResult::Pass; };
    };

    class IControllerInput
    {
    public:

        virtual ~IControllerInput() = default;
        virtual mono::InputResult ButtonDown(const event::ControllerButtonDownEvent& event) { return mono::InputResult::Pass; };
        virtual mono::InputResult Axis(const event::ControllerAxisEvent& event) { return mono::InputResult::Pass; };
    };

    class IKeyboardInput
    {
    public:

        virtual ~IKeyboardInput() = default;
        virtual mono::InputResult KeyDown(const event::KeyDownEvent& event) { return mono::InputResult::Pass; };
        virtual mono::InputResult KeyUp(const event::KeyUpEvent& event) { return mono::InputResult::Pass; };
    };


    enum class InputContextBehaviour
    {
        ConsumeIfHandled,
        ConsumeAll,
        PassOnAll
    };

    struct InputContext
    {
        bool enabled;
        int priority;
        InputContextBehaviour behaviour;
        IMouseInput* mouse_input;
        IControllerInput* controller_input;
        IKeyboardInput* keyboard_input;
    };

    class InputSystem : public mono::IGameSystem
    {
    public:

        InputSystem(mono::EventHandler* event_handler);
        ~InputSystem();

        uint32_t Id() const override;
        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

        InputContext* CreateContext(int priority, InputContextBehaviour context_behaviour);
        void ReleaseContext(InputContext* context);

    private:

        template <typename T>
        inline void ForEachContext(T&& callable)
        {
            for(InputContext* context : m_contexts)
            {
                if(!context->enabled)
                    continue;

                const InputResult input_result = callable(context);
                if(context->behaviour == InputContextBehaviour::ConsumeIfHandled && input_result == InputResult::Handled)
                    break;

                if(context->behaviour == InputContextBehaviour::ConsumeAll)
                    break;
            }
        }

        mono::EventResult OnMouseMotionEvent(const event::MouseMotionEvent& event);
        mono::EventResult OnMouseDownEvent(const event::MouseDownEvent& event);
        mono::EventResult OnMouseUpEvent(const event::MouseUpEvent& event);
        mono::EventResult OnMouseWheelEvent(const event::MouseWheelEvent& event);
        mono::EventResult OnKeyDown(const event::KeyDownEvent& event);
        mono::EventResult OnKeyUp(const event::KeyUpEvent& event);
        mono::EventResult OnControllerButton(const event::ControllerButtonDownEvent& event);
        mono::EventResult OnControllerAxis(const event::ControllerAxisEvent& event);

        mono::EventHandler* m_event_handler;
        std::vector<InputContext*> m_contexts;

        mono::EventToken<event::MouseMotionEvent> m_mouse_motion_token;
        mono::EventToken<event::MouseDownEvent> m_mouse_down_token;
        mono::EventToken<event::MouseUpEvent> m_mouse_up_token;
        mono::EventToken<event::MouseWheelEvent> m_mouse_wheel_token;
        mono::EventToken<event::KeyDownEvent> m_key_down_token;
        mono::EventToken<event::KeyUpEvent> m_key_up_token;
        mono::EventToken<event::ControllerButtonDownEvent> m_controller_down_token;
        mono::EventToken<event::ControllerAxisEvent> m_controller_axis_token;
    };
}
