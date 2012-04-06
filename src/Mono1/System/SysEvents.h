//
//  SysEvents.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef SYSTEM_EVENTS_H
#define SYSTEM_EVENTS_H

#include "MonoFwd.h"

namespace Events
{
    void ProcessSystemEvents(mono::IInputHandlerPtr handler);
}

#endif
