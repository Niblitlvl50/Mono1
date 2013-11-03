//
//  CMObject.h
//  Mono1
//
//  Created by Niblit on 2012-08-24.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "CMFwd.h"
#include <vector>

namespace cm
{
    struct Object
    {
        IBodyPtr body;
        std::vector<IShapePtr> shapes;
    };
}

