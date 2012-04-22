//
//  TestZone.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "ZoneBase.h"

namespace game
{
    class TestZone : public mono::ZoneBase
    {
    public:
        
        TestZone();
    
    private:
        
        virtual bool IsDone() const;
        
        mono::IEntityCollection mEntities;
        
        unsigned int mStartTime;
    };
}

