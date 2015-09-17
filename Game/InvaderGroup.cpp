/*
 *  TriangleObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "InvaderGroup.h"
#include "InvaderEntity.h"
#include "IRenderer.h"

using namespace game;

InvaderGroup::InvaderGroup()
{
    mPosition = math::Vector2f(0.0f, -30.0f);
    mScale = math::Vector2f(20.0f, 20.0f);
    
    AddChild(std::make_shared<InvaderEntity>(-1, -1));
    AddChild(std::make_shared<InvaderEntity>(1, 1));
}

void InvaderGroup::Draw(mono::IRenderer& renderer) const
{
    const std::vector<math::Vector2f> points = { math::Vector2f(0, 0) };
    constexpr mono::Color color(0.0, 0.0, 0.0);
    constexpr float size = 5.0f;

    renderer.DrawPoints(points, color, size);
}

void InvaderGroup::Update(unsigned int delta)
{
    mRotation += (delta * 0.1f);
    if(mRotation > 360.0f)
        mRotation -= 360.0f;
}
