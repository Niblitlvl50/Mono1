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
#include "EntityBase.h"

#include "SysOpenGL.h"
#include "Quad.h"
#include "Texture.h"


using namespace game;

namespace
{
    struct ZoneBounds : mono::EntityBase
    {
        ZoneBounds(const Math::Quad& bounds)
            : mBounds(bounds)
        { }        
        virtual void Draw(mono::IRenderer&) const
        {
            mono::Texture::Clear();

            glColor3f(1.0f, 0.0f, 0.0f);
            
            glBegin(GL_LINE_LOOP);
            glVertex2f(mBounds.mA.mX, mBounds.mA.mY);
            glVertex2f(mBounds.mA.mX, mBounds.mA.mY + mBounds.mB.mY);
            glVertex2f(mBounds.mA.mX + mBounds.mB.mX, mBounds.mA.mY + mBounds.mB.mY);
            glVertex2f(mBounds.mA.mX + mBounds.mB.mX, mBounds.mA.mY);
            glEnd();            
        }
        virtual Math::Quad BoundingBox() const
        {
            return mBounds;
        }
        virtual void Update(unsigned int delta)
        { }
        
        const Math::Quad mBounds;
    };
    
}

void TestZone::OnLoad(mono::ICameraPtr camera)
{    
    mono::IEntityPtr dude(new AnimatedDude(100.0f, 50.0f));
    
    AddEntityToLayer(BACKGROUND, mono::IEntityPtr(new ZoneBounds(Math::Quad(0.0f, 0.0f, 800.0f, 600.0f))));
    AddEntityToLayer(BACKGROUND, mono::IEntityPtr(new TriangleObject));
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new OscillatingLine));
    AddEntityToLayer(MIDDLEGROUND, dude);
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new Explosion));
        
    //AddEntityToLayer(BACKGROUND, mono::IEntityPtr(new Monster));
    
    camera->SetPosition(dude->Position());
    camera->Follow(dude);
}

void TestZone::OnUnload()
{ }

