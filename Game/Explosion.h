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
    struct ExplosionConfiguration
    {
        math::Vector position;
        float scale = 20.0f;
        float rotation = 0.0f;
        const char* sprite_file;
    };

    class Explosion : public mono::EntityBase
    {
    public:
        
        Explosion(const ExplosionConfiguration& config, mono::EventHandler& event_handler);
        virtual void Update(unsigned int delta);
        virtual void Draw(mono::IRenderer& renderer) const;

    private:

        mono::ISpritePtr m_sprite;
    };
}
