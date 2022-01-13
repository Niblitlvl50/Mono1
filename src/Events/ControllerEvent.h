
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
        ControllerButtonDownEvent(int controller_id, System::ControllerButton button)
            : controller_id(controller_id)
            , button(button)
        { }

        const int controller_id;
        const System::ControllerButton button;
    };
}
