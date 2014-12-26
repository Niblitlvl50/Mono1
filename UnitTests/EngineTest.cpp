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
#include "IShaderFactory.h"
#include "IColorShader.h"
#include "ITextureShader.h"

#include "Vector2f.h"
#include "Quad.h"

#include "EventHandler.h"
#include "QuitEvent.h"

namespace
{
    struct MocWindow : mono::IWindow
    {
        MocWindow(mono::EventHandler& handler)
            : mHandler(handler),
              mSize(640, 480)
        { }
        virtual void SurfaceChanged(int width, int height)
        { }
        virtual void Activated(bool activated)
        { }
        virtual void SwapBuffers() const
        {
            mSwapBuffersCalled = true;
            mHandler.DispatchEvent(Event::QuitEvent());
        }
        virtual void MakeCurrent()
        {
            mMakeCurrentCalled = true;
        }
        virtual void SetBackgroundColor(const mono::Color& color)
        { }
        virtual const math::Vector2f& Size() const
        {
            return mSize;
        }

        mono::EventHandler& mHandler;
        math::Vector2f mSize;

        bool mMakeCurrentCalled = false;
        mutable bool mSwapBuffersCalled = false;
    };

    struct MocCamera : mono::ICamera
    {
        MocCamera()
            : mViewport(0.0f, 0.0f, 100.0f, 50.0f)
        { }
        virtual void Update(unsigned int delta)
        {
            mUpdateCalled = true;
        }
        virtual void Follow(const mono::IEntityPtr entity)
        { }
        virtual void Unfollow()
        {
            mUnfollowCalled = true;
        }
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
        bool mUpdateCalled = false;
        bool mUnfollowCalled = false;
    };
    
    struct MocZone : mono::IZone
    {
        MocZone()
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
        
        bool mAcceptCalled = false;
        bool mOnLoadCalled = false;
        bool mOnUnloadCalled = false;
    };

    class NullColorShader : public mono::IColorShader
    {
    public:
        
        virtual void Use()
        { }
        virtual void Clear()
        { }
        virtual unsigned int GetShaderId() const
        {
            return 0;
        }
        virtual void LoadMatrices(const math::Matrix& projection, const math::Matrix& modelview)
        { }

        virtual int GetPositionAttributeLocation() const
        {
            return 0;
        }

        virtual int GetColorAttributeLocation() const
        {
            return 0;
        }
    };

    class NullTextureShader : public mono::ITextureShader
    {
    public:
        virtual void Use()
        { }
        virtual void Clear()
        { }
        virtual unsigned int GetShaderId() const
        {
            return 0;
        }
        virtual void LoadMatrices(const math::Matrix& projection, const math::Matrix& modelview)
        { }

        virtual int GetPositionAttributeLocation() const
        {
            return 0;
        }

        virtual int GetTextureAttributeLocation() const
        {
            return 0;
        }

        virtual void SetShade(const mono::Color& color)
        { }
        virtual void SetAlphaTexture(bool isAlpha)
        { }
    };

    class NullFactory : public mono::IShaderFactory
    {
    public:
        virtual std::shared_ptr<mono::ITextureShader> CreateTextureShader() const
        {
            return std::make_shared<NullTextureShader>();
        }

        virtual std::shared_ptr<mono::IColorShader> CreateColorShader() const
        {
            return std::make_shared<NullColorShader>();
        }
    };
}

TEST(EngineTest, Basic)
{
    mono::EventHandler handler;
    NullFactory factory;
    shaderFactory = &factory;

    std::shared_ptr<MocWindow> window = std::make_shared<MocWindow>(handler);
    std::shared_ptr<MocCamera> camera = std::make_shared<MocCamera>();
    std::shared_ptr<MocZone> zone = std::make_shared<MocZone>();
    
    {
        mono::Engine engine(60.0f, window, camera, handler);
        EXPECT_NO_THROW(engine.Run(zone));
    }

    EXPECT_TRUE(window->mMakeCurrentCalled);
    EXPECT_TRUE(window->mSwapBuffersCalled);

    EXPECT_TRUE(camera->mUpdateCalled);
    EXPECT_TRUE(camera->mUnfollowCalled);
    
    EXPECT_TRUE(zone->mAcceptCalled);
    EXPECT_TRUE(zone->mOnLoadCalled);
    EXPECT_TRUE(zone->mOnUnloadCalled);
}

