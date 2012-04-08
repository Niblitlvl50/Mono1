//
//  TestZone.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef GAME_TESTZONE_H
#define GAME_TESTZONE_H

#include "IZone.h"

namespace game
{
    class TestZone : public mono::IZone
    {
    public:
        
        TestZone();
    
    private:
        
        virtual void Accept(mono::IRenderer& renderer);
        virtual bool IsDone() const;
        
        mono::IEntityCollection mEntities;
        
        unsigned int mStartTime;
    };
}

#endif

