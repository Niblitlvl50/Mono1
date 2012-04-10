//
//  AnimatedDude.h
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef GAME_ANIMATEDDUDE_H
#define GAME_ANIMATEDDUDE_H

#include "EntityBase.h"
#include "AnimatedSprite.h"

namespace game
{
	class AnimatedDude : public mono::EntityBase
	{
	public:
		
		AnimatedDude(float x, float y);
        
    private:
        
		virtual void Draw() const;
        virtual void Update(unsigned int delta);
        
        float mX;
        float mY;
        mono::AnimatedSprite mSprite;
	};
}

#endif

