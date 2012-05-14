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
                
        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();        
    };
}

