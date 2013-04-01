//
//  Moon.h
//  Mono1
//
//  Created by Niblit on 2012-10-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "PhysicsEntityBase.h"
#include "Sprite.h"

namespace game
{
    class Moon : public mono::PhysicsEntityBase
    {
    public:
        Moon(float x, float y, float radius);

    private:
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        mono::Sprite mSprite;
    };
}