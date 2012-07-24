//
//  Head.h
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
    class Head : public mono::EntityBase
    {
    public:
    
        Head(float x, float y);
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
    
    private:
        
        mono::Sprite mHead;
        mono::Sprite mJaw;
    };
}