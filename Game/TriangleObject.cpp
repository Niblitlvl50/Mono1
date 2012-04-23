/*
 *  TriangleObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TriangleObject.h"
#include "SysOpenGL.h"
#include "SquareObject.h"
#include "Texture.h"

using namespace game;

TriangleObject::TriangleObject()
{
    mPosition = Math::Vector2f(400.0f, 300.0f);
    mScale = 20.0f;
    mRotation = 1.0f;
    
    AddChild(mono::IEntityPtr(new game::SquareObject(1.0f, 1.0f)));
    AddChild(mono::IEntityPtr(new game::SquareObject(-1.0f, -1.0f)));
}

void TriangleObject::Draw() const
{ }

void TriangleObject::Update(unsigned int delta)
{
    mRotation += (delta * 0.1f);
    if(mRotation > 360.0f)
        mRotation -= 360.0f;
}
