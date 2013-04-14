//
//  Shuttle.h
//  Mono1
//
//  Created by Niblit on 2012-08-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "PhysicsEntityBase.h"
#include "Sprite.h"
#include "ShuttleController.h"
#include "CMIBody.h"

namespace game
{

    class Shuttle : public mono::PhysicsEntityBase, public cm::ICollisionHandler
    {
        friend class ShuttleController;

    public:
        
        Shuttle(float x, float y);

    private:

        void Fire();
        
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        
        virtual void OnCollideWith(cm::IBodyPtr body);
        virtual void OnPostStep();
        
        mono::Sprite mSprite;
        ShuttleController mController;
    };
    
}

