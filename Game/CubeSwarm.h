//
//  CubeSwarm.h
//  MonoOSX
//
//  Created by Niklas Damberg on 24/03/15.
//
//

#pragma once

#include "EntityBase.h"
#include "Quad.h"
#include "Vector2f.h"
#include "Color.h"
#include <vector>

namespace game
{
    class CubeSwarm : public mono::EntityBase
    {
    public:

        CubeSwarm();

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

    private:

        struct Cube
        {
            Cube()
            { }

            Cube(const math::Vector2f& position, float size)
                : quad(position.x, position.y, position.x + size, position.y + size)
            { }

            math::Quad quad;
            float rotation;
            mono::Color::RGBA color;
        };

        Cube GenerateCube();

        std::vector<Cube> mCubes;
    };
}
