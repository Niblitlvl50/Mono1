//
//  PhysicsGrid.h
//  MonoOSX
//
//  Created by Niklas Damberg on 17/11/14.
//
//

#pragma once

#include "PhysicsEntityBase.h"
#include "Quad.h"

namespace game
{
    class PhysicsGrid : public mono::PhysicsEntityBase
    {
    public:

        PhysicsGrid(const math::Quad& bounds);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        virtual math::Quad BoundingBox() const;

    private:

        const math::Quad mBounds;
        std::vector<math::Vector2f> mGridVertices;
    };
}
