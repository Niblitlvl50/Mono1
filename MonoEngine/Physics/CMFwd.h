//
//  CMFwd.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include <tr1/memory>
#include <vector>

struct cpSpace;
struct cpBody;
struct cpShape;

namespace Math
{
    struct Vector2f;
}

typedef std::vector<Math::Vector2f> Vector2fCollection;

namespace cm
{
    struct Object;
    struct IBody;
    struct IShape;
    
    typedef std::tr1::shared_ptr<IBody> IBodyPtr;
    typedef std::tr1::shared_ptr<IShape> IShapePtr;
    
    typedef std::vector<IShapePtr> IShapeCollection;
}
