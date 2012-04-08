/*
 *  SquareObject.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GAME_SQUAREOBJECT_H
#define GAME_SQUAREOBJECT_H

#include "EntityBase.h"
#include "Sprite.h"

namespace game
{
	
	class SquareObject : public mono::EntityBase
	{
	public:
		
		SquareObject(float x, float y, float z);
				
	private:
        
        virtual void Draw() const;
        virtual void Update(unsigned int delta);
        		
		float mPosX;
		float mPosY;
		float mPosZ;
        
        mono::Sprite mSprite;
	};

}

#endif

