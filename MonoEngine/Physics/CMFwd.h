//
//  CMFwd.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include <memory>
#include <functional>

struct cpBody;
struct cpShape;

namespace mono
{
    struct Object;
    struct IBody;
    struct IShape;

    using IBodyPtr = std::shared_ptr<IBody>;
    using IShapePtr = std::shared_ptr<IShape>;
    using BodyFunc = std::function<void (const IBodyPtr&)>;
}
