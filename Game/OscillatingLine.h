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
    class OscillatingLine : public mono::EntityBase
    {
    public:
        
        OscillatingLine();
        
    private:
        
        virtual void Draw() const;
        virtual void Update(unsigned int delta);
        
        float mDelta;
    };
}

