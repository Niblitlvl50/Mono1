//
//  Monster.h
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
    class Monster : public mono::EntityBase
    {
    public:
        
        Monster();
        virtual void Draw(mono::IRenderer&) const;
        virtual void Update(unsigned int delta);
        
    private:
    
        mono::Sprite mBody;
        
        mono::IEntityPtr mHead;
        mono::IEntityPtr mRightArm;
        mono::IEntityPtr mLeftArm;
        mono::IEntityPtr mRightLeg;
        mono::IEntityPtr mLeftLeg;
    };
}
