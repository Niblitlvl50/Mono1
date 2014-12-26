//
//  PhysicsGrid.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 17/11/14.
//
//

#include "PhysicsGrid.h"
#include "CMFactory.h"
#include "CMIShape.h"
#include "IRenderer.h"
#include "Color.h"

#include <functional>

using namespace game;

namespace
{
    std::vector<math::Vector2f> BuildGridVertices(const math::Quad& bounds)
    {
        constexpr float cellSize = 50.0f;

        const float width = bounds.mB.mX - bounds.mA.mX;
        const float height = bounds.mB.mY - bounds.mA.mY;

        const float cellsWidth = width / cellSize;
        const float cellsHeight = height / cellSize;

        std::vector<math::Vector2f> vertices;

        for(int index = 1; index < cellsWidth; ++index)
        {
            const float x = bounds.mA.mX + index * cellSize;

            vertices.push_back(math::Vector2f(x, bounds.mA.mY));
            vertices.push_back(math::Vector2f(x, bounds.mB.mY));
        }

        for(int index = 1; index < cellsHeight; ++index)
        {
            const float y = bounds.mA.mY + index * cellSize;

            vertices.push_back(math::Vector2f(bounds.mA.mX, y));
            vertices.push_back(math::Vector2f(bounds.mB.mX, y));
        }

        return vertices;
    }
}

PhysicsGrid::PhysicsGrid(const math::Quad& bounds)
    : mBounds(bounds)
{
    mPhysicsObject.body = cm::Factory::CreateStaticBody();

    constexpr float radius = 2.0f;

    const math::Vector2f first = mBounds.mA;
    const math::Vector2f second = math::Vector2f(mBounds.mA.mX, mBounds.mB.mY);
    const math::Vector2f third = mBounds.mB;
    const math::Vector2f fourth = math::Vector2f(mBounds.mB.mX, mBounds.mA.mY);

    mPhysicsObject.shapes.push_back(cm::Factory::CreateShape(mPhysicsObject.body, first, second, radius));
    mPhysicsObject.shapes.push_back(cm::Factory::CreateShape(mPhysicsObject.body, second, third, radius));
    mPhysicsObject.shapes.push_back(cm::Factory::CreateShape(mPhysicsObject.body, third, fourth, radius));
    mPhysicsObject.shapes.push_back(cm::Factory::CreateShape(mPhysicsObject.body, fourth, first, radius));

    using namespace std::placeholders;

    std::for_each(mPhysicsObject.shapes.begin(),
                  mPhysicsObject.shapes.end(),
                  std::bind(&cm::IShape::SetElasticity, _1, 0.9f));

    mGridVertices = BuildGridVertices(mBounds);
}

void PhysicsGrid::Draw(mono::IRenderer& renderer) const
{
    constexpr float quadWidth = 2.0f;
    renderer.DrawQuad(mBounds, mono::Color(1, 1, 1, 1), quadWidth);

    const mono::Color color(1.0f, 1.0f, 1.0f, 0.2f);
    constexpr float lineWidth = 1.0f;
    renderer.DrawLines(mGridVertices, color, lineWidth);
}

void PhysicsGrid::Update(unsigned int delta)
{ }

math::Quad PhysicsGrid::BoundingBox() const
{
    return mBounds;
}


