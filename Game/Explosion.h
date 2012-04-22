//
//  Explosion.h
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "EntityBase.h"
#include "Sprite.h"

namespace game
{
    class Explosion : public mono::EntityBase
    {
    public:
        
        Explosion();
        
        virtual void Update(unsigned int delta);
        virtual void Draw() const;
        
    private:
        
        mono::Sprite mSprite;
    };
}
