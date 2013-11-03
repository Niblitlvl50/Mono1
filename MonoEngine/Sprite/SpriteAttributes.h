//
//  SpriteAttributes.h
//  Mono1
//
//  Created by Niblit on 2013-03-20.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <string>

namespace mono
{
    struct FindNoLoopAttribute
    {
        bool operator () (const std::string& input) const
        {
            return input == "no-loop";
        }
    };    
}
