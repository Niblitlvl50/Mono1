//
//  Time.h
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#pragma once

namespace Time
{
    //! Gets the system time in milliseconds
    unsigned int GetMilliseconds();

    //! Sleep for some milliseconds
    void Sleep(unsigned int milliseconds);
}

