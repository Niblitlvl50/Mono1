//
//  Arm.h
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
    class Arm : public mono::EntityBase
    {
    public:
        
        Arm(float x, float y);
        
        virtual void Draw() const;
        virtual void Update(unsigned int delta);
        
    private:
        
        mono::IEntityPtr mUpper;
        mono::IEntityPtr mLower;
        mono::IEntityPtr mHand;    
    };
}