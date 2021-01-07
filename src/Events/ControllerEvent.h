
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
}
