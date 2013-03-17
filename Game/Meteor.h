//
//  Meteor.h
//  Mono1
//
//  Created by Niblit on 2013-01-27.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "EntityBase.h"
#include "Sprite.h"
#include "CMObject.h"

namespace game
{
    class Meteor : public mono::EntityBase
    {
    public:
        Meteor(float x, float y);
    
        cm::Object mPhysicsObject;

    private:
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        
        mono::Sprite mSprite;
    };
}