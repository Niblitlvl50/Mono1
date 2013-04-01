//
//  FireBullet.h
//  Mono1
//
//  Created by Niblit on 2013-03-30.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "EntityBase.h"
#include "Sprite.h"

namespace game
{
    class FireBullet : public mono::EntityBase
    {
    public:
        
        FireBullet(const math::Vector2f& start, float rotation, const math::Vector2f& speed);
        void Update(unsigned int delta);
        void Draw(mono::IRenderer& renderer) const;
        
    private:
        
        mono::Sprite mSprite;
        const math::Vector2f mSpeed;
    };
}

