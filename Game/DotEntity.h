//
//  OscillatingLine.h
//  Monolith1
//
//  Created by Niblit on 2012-01-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "EntityBase.h"

namespace game
{
    class DotEntity : public mono::EntityBase
    {
    public:
        
        DotEntity();
        
    private:
        
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        
        math::Vector mBase;
        float mDelta;
    };
}

