
#pragma once

#include "MonoFwd.h"
#include "IGameSystem.h"
#include "InputInterfaces.h"
#include "InputSystemTypes.h"

#include <vector>

namespace mono
{
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
        InputContextType most_recent_input;
        IMouseInput* mouse_input;
        IControllerInput* controller_input;
        IKeyboardInput* keyboard_input;
        const char* debug_context;
    };

    class InputSystem : public mono::IGameSystem
    {
    public:

        InputSystem(mono::EventHandler* event_handler);
        ~InputSystem();

        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

        InputContext* CreateContext(int priority, InputContextBehaviour context_behaviour, const char* debug_context);
        void ReleaseContext(InputContext* context);

        void EnableContext(InputContext* input_context);
        void DisableContext(InputContext* input_context);

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
