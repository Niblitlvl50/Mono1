//
//  TestZone.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TestZone.h"
#include "TriangleObject.h"
#include "OscillatingLine.h"
#include "AnimatedDude.h"
#include "Explosion.h"
#include "Monster.h"
#include "ICamera.h"


using namespace game;


void TestZone::OnLoad(mono::ICameraPtr camera)
{
    mono::IEntityPtr dude(new AnimatedDude(100.0f, 50.0f));
    
    AddEntityToLayer(BACKGROUND, mono::IEntityPtr(new TriangleObject));
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new OscillatingLine));
    AddEntityToLayer(MIDDLEGROUND, dude);
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new Explosion));
    
    //AddEntityToLayer(BACKGROUND, mono::IEntityPtr(new Monster));
    
    camera->Follow(dude);
}

void TestZone::OnUnload()
{ }

