//
//  Meteor.h
//  Mono1
//
//  Created by Niblit on 2013-01-27.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "PhysicsEntityBase.h"
#include "MonoPtrFwd.h"

namespace game
{
    class Meteor : public mono::PhysicsEntityBase
    {
    public:
        Meteor(float x, float y);
    
    private:
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        
        mono::ISpritePtr mSprite;
    };
}

