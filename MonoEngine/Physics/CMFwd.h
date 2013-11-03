//
//  CMFwd.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include <memory>

struct cpBody;
struct cpShape;

namespace cm
{
    struct Object;
    
    struct IBody;
    typedef std::shared_ptr<IBody> IBodyPtr;

    struct IShape;
    typedef std::shared_ptr<IShape> IShapePtr;
}
