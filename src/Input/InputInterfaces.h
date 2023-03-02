
#pragma once

#include "EventHandler/EventToken.h"
#include "Events/MouseEvent.h"
#include "Events/ControllerEvent.h"
#include "Events/KeyEvent.h"

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
        virtual mono::InputResult UpdatedControllerState(const System::ControllerState& updated_state) { return mono::InputResult::Pass; };
    };

    class IKeyboardInput
    {
    public:

        virtual ~IKeyboardInput() = default;
        virtual mono::InputResult KeyDown(const event::KeyDownEvent& event) { return mono::InputResult::Pass; };
        virtual mono::InputResult KeyUp(const event::KeyUpEvent& event) { return mono::InputResult::Pass; };
    };
}
