//
//  OscillatingLine.h
//  Monolith1
//
//  Created by Niblit on 2012-01-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef GAME_OSCILLATINGLINE_H
#define GAME_OSCILLATINGLINE_H

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


#endif

