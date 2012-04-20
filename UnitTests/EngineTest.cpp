//
//  EngineTest.cpp
//  UnitTests
//
//  Created by Niblit on 2012-04-08.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "gtest.h"

#include "Engine.h"
#include "MonoFwd.h"
#include "IWindow.h"
#include "ICamera.h"
#include "IZone.h"
#include "GameController.h"

#include "EventHandler.h"
#include "QuitEvent.h"
#include "ActivatedEvent.h"
#include "SurfaceChangedEvent.h"

namespace
{
    struct MocWindow : mono::IWindow
    {
        MocWindow()
            : mSurfaceChangedCalled(false),
              mActivatedCalled(false),
              mFrameDrawn(false)
        { }
        
        virtual void SurfaceChanged(int width, int height)
        {
            mSurfaceChangedCalled = (width == 100 && height == 100);
        }
        virtual void Activated(bool activated)
        {
            mActivatedCalled = activated;
        }
        virtual void DrawFrame(mono::IRenderer& renderer) const
        {
            Event::ActivatedEvent activated(true);
            mono::EventHandler::DispatchEvent(activated);
            
            Event::SurfaceChangedEvent surfaceChanged(100, 100);
            mono::EventHandler::DispatchEvent(surfaceChanged);
            
            Event::QuitEvent event;
            mono::EventHandler::DispatchEvent(event);

            mFrameDrawn = true;
        }
        virtual int GetWidth() const
        {
            return 10;
        }
        virtual int GetHeight() const
        {
            return 10;
        }

        bool mSurfaceChangedCalled;
        bool mActivatedCalled;
        mutable bool mFrameDrawn;
    };
    
    struct MocCamera : mono::ICamera
    { };
    
    struct MocZone : mono::IZone
    {
        MocZone()
            : mAcceptCalled(false),
              mIsDoneCalled(false)
        { }
        
        virtual void Accept(mono::IRenderer& renderer)
        {
            mAcceptCalled = true;
        }
        virtual bool IsDone() const
        {
            mIsDoneCalled = true;
            
            return false;
        }
        
        bool mAcceptCalled;
        mutable bool mIsDoneCalled;
    };
}

TEST(EngineTest, Basic)
{
    MocWindow* mocWin = new MocWindow;    
    mono::IWindowPtr window(mocWin);
    
    mono::ICameraPtr camera(new MocCamera);
    mono::Engine engine(60.0f, window, camera);
    
    MocZone* mocZone = new MocZone;
    mono::IZonePtr zone(mocZone);
    
    mono::GameControllerInstance().InsertZone(zone);

    EXPECT_NO_THROW(engine.Run());
    
    EXPECT_TRUE(mocWin->mSurfaceChangedCalled);
    EXPECT_TRUE(mocWin->mActivatedCalled);
    EXPECT_TRUE(mocWin->mFrameDrawn);
    
    EXPECT_TRUE(mocZone->mAcceptCalled);
    EXPECT_TRUE(mocZone->mIsDoneCalled);
}

