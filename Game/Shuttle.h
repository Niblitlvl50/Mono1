//
//  Shuttle.h
//  Mono1
//
//  Created by Niblit on 2012-08-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "EntityBase.h"
#include "Sprite.h"
#include "CMObject.h"
#include "ShuttleController.h"

namespace game
{

    class Shuttle : public mono::EntityBase
    {
    public:
        
        Shuttle(float x, float y);
        
        cm::Object mPhysicsObject;

    private:

        friend class ShuttleController;
        
        void Fire();
        
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        
        mono::Sprite mSprite;
        ShuttleController mController;
    };
    
}

