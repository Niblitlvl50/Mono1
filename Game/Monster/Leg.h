//
//  Leg.h
//  Mono1
//
//  Created by Niblit on 2012-05-01.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "EntityBase.h"
#include "Sprite.h"

namespace game
{
    class Leg : public mono::EntityBase
    {
    public:
        
        Leg(float x, float y);
        
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        
    private:
        
        mono::Sprite mUpper;
        mono::Sprite mLower;
        mono::Sprite mFoot;
    };
}
