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
#include "Color.h"

namespace game
{
    class InvaderEntity : public mono::EntityBase
    {
    public:

        InvaderEntity(float x, float y);

    private:

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        mono::Sprite mSprite;
        mono::Color::HSL mColor;
    };

}

