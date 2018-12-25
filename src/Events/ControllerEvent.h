
#pragma once

namespace event
{
    struct ControllerAddedEvent
    {
        ControllerAddedEvent(int controller_id)
            : id(controller_id)
        { }

        const int id;
    };

    struct ControllerRemovedEvent
    {
        ControllerRemovedEvent(int controller_id)
            : id(controller_id)
        { }

        const int id;
    };
}
