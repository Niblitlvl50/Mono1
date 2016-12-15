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
#include "Math/MathFunctions.h"

using namespace game;

InvaderGroup::InvaderGroup(const math::Vector& position)
{
    mPosition = position;
    mScale = math::Vector(20.0f, 20.0f);
    
    AddChild(std::make_shared<InvaderEntity>(-1, -1));
    AddChild(std::make_shared<InvaderEntity>(1, 1));
}

void InvaderGroup::Draw(mono::IRenderer& renderer) const
{
    const std::vector<math::Vector> points = { math::Vector(0, 0) };
    constexpr mono::Color::RGBA color(0.0, 0.0, 0.0);
    constexpr float size = 5.0f;

    renderer.DrawPoints(points, color, size);
}

void InvaderGroup::Update(unsigned int delta)
{
    constexpr float TWO_PI = math::PI() * 2.0;

    mRotation += (delta * 0.001f);
    if(mRotation > TWO_PI)
        mRotation -= TWO_PI;
}
