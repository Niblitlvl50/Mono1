/*
 *  SquareObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SquareObject.h"
#include "SysOpenGL.h"

#include <string>

using namespace game;

SquareObject::SquareObject(float x, float y, float z)
	: mPosX(x),
	  mPosY(y),
	  mPosZ(z),
      mSprite("small_craft.png", 1.0f, 1.0f)
{ }

void SquareObject::Draw() const
{
    mSprite.DrawAt(mPosX, mPosY);
	EntityBase::Draw();
}

void SquareObject::Update(unsigned int delta)
{
    EntityBase::Update(delta);
}
