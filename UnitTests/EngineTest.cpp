//
//  EngineTest.cpp
//  UnitTests
//
//  Created by Niblit on 2012-04-08.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "gtest/gtest.h"

#include "Engine.h"
#include "MonoFwd.h"
#include "IWindow.h"
#include "ICamera.h"
#include "IZone.h"

#include "Vector2f.h"
#include "Quad.h"

namespace
{    
    struct MocCamera : mono::ICamera
    {
        MocCamera()
            : mViewport(0.0f, 0.0f, 100.0f, 50.0f)
        { }
        virtual void Update(unsigned int delta)
        { }
        virtual void Follow(const mono::IEntityPtr entity)
        { }
        virtual void Unfollow()
        { }
        virtual const math::Quad& GetViewport() const
        {
            return mViewport;
        }
        virtual void SetPosition(const math::Vector2f& position)
        { }
        
        math::Quad mViewport;
    };
    
    struct MocZone : mono::IZone
    {
        MocZone()
            : mAcceptCalled(false),
              mOnLoadCalled(false),
              mOnUnloadCalled(false)
        { }
        virtual void Accept(mono::IRenderer& renderer)
        {
            mAcceptCalled = true;
        }
        virtual void OnLoad(mono::ICameraPtr camera)
        {
            mOnLoadCalled = true;
        }
        virtual void OnUnload()
        {
            mOnUnloadCalled = true;
        }
        
        bool mAcceptCalled;
        bool mOnLoadCalled;
        bool mOnUnloadCalled;
    };
}

/*
TEST(EngineTest, Basic)
{
    MocCamera* mocCamera = new MocCamera;
    mono::ICameraPtr camera(comCamera);
    
    MocZone* mocZone = new MocZone;
    mono::IZonePtr zone(mocZone);

    {
        mono::Engine engine(60.0f, window, camera, zone);

        EXPECT_NO_THROW(engine.Run());
    }
    
    EXPECT_TRUE(mocWin->mSurfaceChangedCalled);
    EXPECT_TRUE(mocWin->mActivatedCalled);
    EXPECT_TRUE(mocWin->mSwapBuffersCalled);
    
    EXPECT_TRUE(mocZone->mAcceptCalled);
    EXPECT_TRUE(mocZone->mOnLoadCalled);
    EXPECT_TRUE(mocZone->mOnUnloadCalled);
}
*/

