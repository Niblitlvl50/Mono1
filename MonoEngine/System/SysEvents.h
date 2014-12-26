//
//  SysEvents.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoPtrFwd.h"

namespace Events
{
    //! Process the system events, input and other events
    //! \param handler An input handler that takes care of the events
    void ProcessSystemEvents(mono::IInputHandlerPtr handler);
}
