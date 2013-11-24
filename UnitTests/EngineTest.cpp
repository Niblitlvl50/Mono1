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

#include "EventHandler.h"
#include "QuitEvent.h"

namespace
{
    struct MocWindow : mono::IWindow
    {
        MocWindow()
            : mSwapBuffersCalled(false),
              mSize(640, 480)
        { }
        virtual void SurfaceChanged(int width, int height)
        { }
        virtual void Activated(bool activated)
        { }
        virtual void SwapBuffers() const
        {
            mSwapBuffersCalled = true;
            mono::EventHandler::DispatchEvent(Event::QuitEvent());
        }
        virtual const math::Vector2f& Size() const
        {
            return mSize;
        }
        
        mutable bool mSwapBuffersCalled;
        math::Vector2f mSize;
    };
    
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
        virtual math::Quad& GetViewport()
        {
            return mViewport;
        }
        virtual void SetTargetViewport(const math::Quad& target)
        { }
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
        virtual void AddEntity(mono::IEntityPtr entity, int layer)
        { }
        virtual void RemoveEntity(mono::IEntityPtr entity)
        { }
        virtual void AddDrawable(mono::IDrawablePtr drawable, int layer)
        { }
        virtual void RemoveDrawable(mono::IDrawablePtr drawable)
        { }
        virtual void AddUpdatable(mono::IUpdatablePtr updatable)
        { }
        virtual void RemoveUpdatable(mono::IUpdatablePtr updatable)
        { }
        
        bool mAcceptCalled;
        bool mOnLoadCalled;
        bool mOnUnloadCalled;
    };
}

/*
TEST(EngineTest, Basic)
{
    std::shared_ptr<MocWindow> window = std::make_shared<MocWindow>();
    std::shared_ptr<MocCamera> camera = std::make_shared<MocCamera>();
    std::shared_ptr<MocZone> zone = std::make_shared<MocZone>();
    
    {
        mono::Engine engine(60.0f, window, camera, zone);
        EXPECT_NO_THROW(engine.Run());
    }
    
    //EXPECT_TRUE(window->mSurfaceChangedCalled);
    //EXPECT_TRUE(window->mActivatedCalled);
    EXPECT_TRUE(window->mSwapBuffersCalled);
    
    EXPECT_TRUE(zone->mAcceptCalled);
    EXPECT_TRUE(zone->mOnLoadCalled);
    EXPECT_TRUE(zone->mOnUnloadCalled);
}
 */

