//
//  PathPoint.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/13.
//
//

#include "PathPoint.h"
#include "Paths/PathFactory.h"
#include "Paths/IPath.h"
#include "IRenderer.h"
#include "Color.h"

using namespace game;


PathPoint::PathPoint()
    : mElapsedTime(0)
{
    mScale = math::Vector(10, 10);

    const std::vector<math::Vector> points = {
        math::Vector(0, 0),
        math::Vector(0, 100),
        math::Vector(50, 100),
        math::Vector(250, 200),
        math::Vector(30, 30),
        math::Vector(0, 0)
    };

    mPath = mono::CreatePath(math::zeroVec, points);
}

void PathPoint::Draw(mono::IRenderer& renderer) const
{
    const std::vector<math::Vector> points = { math::Vector(0, 0) };
    constexpr mono::Color::RGBA color(0.7, 0.0, 0.0);
    constexpr float size = 14.0f;

    renderer.DrawPoints(points, color, size);
}

void PathPoint::Update(unsigned int delta)
{
    mElapsedTime += delta;
    
    // 10000 == 10 seconds
    const float duration = 10000;
    const float time = mElapsedTime / duration;
    const float position = mPath->Length() * time;
    
    mPosition = mPath->GetPositionByLength(position);

    // Restart the path
    if(mElapsedTime > duration)
        mElapsedTime = 0;
}

