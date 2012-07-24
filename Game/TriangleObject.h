/*
 *  TriangleObject.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "EntityBase.h"

namespace game
{
    class TriangleObject : public mono::EntityBase
    {
    public:

        TriangleObject();

    private:

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
    };
}

