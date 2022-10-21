
#pragma once

namespace event
{
    struct ControllerAddedEvent
    {
        ControllerAddedEvent(int controller_id)
            : controller_id(controller_id)
        { }

        const int controller_id;
    };

    struct ControllerRemovedEvent
    {
        ControllerRemovedEvent(int controller_id)
            : controller_id(controller_id)
        { }

        const int controller_id;
    };
    
    struct ControllerButtonDownEvent
    {
        ControllerButtonDownEvent(int controller_id, System::ControllerButton button, uint32_t timestamp)
            : controller_id(controller_id)
            , button(button)
            , timestamp(timestamp)
        { }

        const int controller_id;
        const System::ControllerButton button;
        const uint32_t timestamp;
    };

    struct ControllerAxisEvent
    {
        ControllerAxisEvent(int controller_id, System::ControllerAxis axis, float value, uint32_t timestamp)
            : controller_id(controller_id)
            , axis(axis)
            , value(value)
            , timestamp(timestamp)
        { }

        const int controller_id;
        const System::ControllerAxis axis;
        const float value;
        const uint32_t timestamp;
    };
}
