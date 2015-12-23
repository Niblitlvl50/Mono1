//
//  Morpher.hpp
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/15.
//
//

#pragma once

#include "EntityBase.h"

namespace game
{
    class Morpher : public mono::EntityBase
    {
    public:

        Morpher();
        
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        std::vector<math::Vector2f> m_shape1;
        std::vector<math::Vector2f> m_shape2;

        float m_morphGrade;

        bool m_direction;
        int m_counter;
    };
}
