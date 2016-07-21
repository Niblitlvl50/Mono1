//
//  Explosion.h
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "EntityBase.h"

namespace game
{
    class Explosion : public mono::EntityBase
    {
    public:
        
        Explosion(mono::EventHandler& event_handler,
                  const math::Vector2f& position,
                  float scale = 20,
                  float rotation = 0);
        
        virtual void Update(unsigned int delta);
        virtual void Draw(mono::IRenderer& renderer) const;

    private:

        mono::ISpritePtr mSprite;
    };
}
