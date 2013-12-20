//
//  FireBullet.h
//  Mono1
//
//  Created by Niblit on 2013-03-30.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "PhysicsEntityBase.h"
#include "Sprite.h"
#include "CMIBody.h"

namespace game
{
    class FireBullet : public mono::PhysicsEntityBase, public cm::ICollisionHandler
    {
    public:
        
        FireBullet(const math::Vector2f& start, float rotation, mono::EventHandler& eventHandler);
        void Update(unsigned int delta);
        void Draw(mono::IRenderer& renderer) const;
        
        virtual void OnCollideWith(cm::IBodyPtr body);
        virtual void OnPostStep();
        
        virtual bool RemoveMe() const;

    private:
        
        mono::Sprite mSprite;
        mono::EventHandler& mEventHandler;
        bool mRemoveMe;
    };
}

