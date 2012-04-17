//
//  AnimatedDude.h
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "EntityBase.h"
#include "Vector2f.h"
#include "AnimatedSprite.h"
#include "AnimatedDudeController.h"

namespace game
{
	class AnimatedDude : public mono::EntityBase
	{
        friend class AnimatedDudeController;
        
	public:
		
		AnimatedDude(float x, float y);
        
    private:
        
		virtual void Draw() const;
        virtual void Update(unsigned int delta);
        
        Math::Vector2f mPosition;
        Math::Vector2f mTarget;
        mono::AnimatedSprite mSprite;
        AnimatedDudeController mController;
        
	};
}

