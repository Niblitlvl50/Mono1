//
//  PathPoint.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/13.
//
//

#include "PathPoint.h"
#include "PathFactory.h"
#include "IPath.h"
#include "IRenderer.h"
#include "Color.h"

using namespace game;


PathPoint::PathPoint()
    : mElapsedTime(0)
{
    mScale = math::Vector2f(10, 10);
    const std::vector<math::Vector2f> points = { math::Vector2f(0, 0),
                                                 math::Vector2f(0, 100),
                                                 math::Vector2f(50, 100),
                                                 math::Vector2f(250, 200),
                                                 math::Vector2f(30, 30),
                                                 math::Vector2f(0, 0) };
    mPath = mono::CreatePath(points);
}

void PathPoint::Draw(mono::IRenderer& renderer) const
{
    const std::vector<math::Vector2f> points = { math::Vector2f(0, 0) };
    const mono::Color color(0.7, 0.0, 0.0, 1.0);
    constexpr float size = 4.0f;

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

