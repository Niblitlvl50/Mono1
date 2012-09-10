//
//  CMShapeFactory.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "CMFwd.h"
#include <limits>

static const float INF = std::numeric_limits<float>::infinity();

namespace cm
{            
    class Factory
    {
    public:
        
        //! Body
        static IBodyPtr CreateBody(float mass, float inertia);
        
        //! Circle shape
        static IShapePtr CreateShape(IBodyPtr body, float radius, const Math::Vector2f& offset, bool staticShape);    
        
        //! Box shape
        static IShapePtr CreateShape(IBodyPtr body, float width, float height, bool staticShape);

        //! Segment shape
        static IShapePtr CreateShape(IBodyPtr body, const Math::Vector2f& first, const Math::Vector2f& second, float radius, bool staticShape);
        
        //! Polygon shape
        static IShapePtr CreateShape(IBodyPtr body, const Vector2fCollection& vertices, const Math::Vector2f& offset, bool staticShape);
    };
}



