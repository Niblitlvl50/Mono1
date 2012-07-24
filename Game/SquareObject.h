/*
 *  SquareObject.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "EntityBase.h"
#include "Sprite.h"

namespace game
{
    
    class SquareObject : public mono::EntityBase
    {
    public:

        SquareObject(float x, float y);

    private:

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        mono::Sprite mSprite;
    };

}

