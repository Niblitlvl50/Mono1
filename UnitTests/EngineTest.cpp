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
#include "Shader/IShaderFactory.h"
#include "Shader/IColorShader.h"
#include "Shader/ITextureShader.h"
#include "Shader/IMorphingShader.h"
#include "Shader/ShaderFunctions.h"

#include "Math/Vector2f.h"
#include "Math/Quad.h"

#include "EventHandler/EventHandler.h"
#include "Events/QuitEvent.h"

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
        virtual void Maximize()
        { }
        virtual void Minimize()
        { }
        virtual void RestoreSize()
        { }
        virtual void SwapBuffers() const
        {
            mSwapBuffersCalled = true;
            mHandler.DispatchEvent(event::QuitEvent());
        }
        virtual void MakeCurrent()
        {
            mMakeCurrentCalled = true;
        }
        virtual void SetBackgroundColor(const mono::Color::RGBA& color)
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
        virtual void Follow(const mono::IEntityPtr& entity, const math::Vector2f& offset)
        { }
        virtual void Unfollow()
        {
            mUnfollowCalled = true;
        }
        virtual math::Quad GetViewport() const
        {
            return mViewport;
        }
        virtual math::Vector2f GetPosition() const
        {
            return mViewport.mA + (mViewport.mB * 0.5f);
        }
        virtual void SetViewport(const math::Quad& viewport)
        { }
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
        virtual void CreateLayer(int layer)
        { }
        virtual void AddEntity(const mono::IEntityPtr& entity, int layer)
        { }
        virtual void RemoveEntity(const mono::IEntityPtr& entity)
        { }
        virtual void AddDrawable(const mono::IDrawablePtr& drawable, int layer)
        { }
        virtual void RemoveDrawable(const mono::IDrawablePtr& drawable)
        { }
        virtual void AddUpdatable(const mono::IUpdatablePtr& updatable)
        { }
        virtual void RemoveUpdatable(const mono::IUpdatablePtr& updatable)
        { }
        virtual mono::IEntityPtr FindEntityFromId(uint id) const
        {
            return nullptr;
        }
        virtual void SchedulePreFrameTask(const std::function<void ()>& task)
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
        virtual unsigned int GetShaderId() const
        {
            return 0;
        }
        virtual void LoadProjectionMatrix(const math::Matrix& projection)
        { }
        virtual void LoadModelViewMatrix(const math::Matrix& modelView)
        { }
        virtual unsigned int GetPositionAttributeLocation() const
        {
            return 0;
        }
        virtual unsigned int GetColorAttributeLocation() const
        {
            return 0;
        }
        virtual void SetPointSize(float size)
        { }
    };

    class NullTextureShader : public mono::ITextureShader
    {
    public:
        virtual void Use()
        { }
        virtual unsigned int GetShaderId() const
        {
            return 0;
        }
        virtual void LoadProjectionMatrix(const math::Matrix& projection)
        { }
        virtual void LoadModelViewMatrix(const math::Matrix& modelView)
        { }
        virtual unsigned int GetPositionAttributeLocation() const
        {
            return 0;
        }
        virtual unsigned int GetTextureAttributeLocation() const
        {
            return 0;
        }
        virtual void SetShade(const mono::Color::RGBA& color)
        { }
        virtual void SetAlphaTexture(bool isAlpha)
        { }
    };

    class NullMorphShader : public mono::IMorphingShader
    {
    public:
        virtual void Use()
        { }
        virtual unsigned int GetShaderId() const
        {
            return 0;
        }
        virtual void LoadProjectionMatrix(const math::Matrix& projection)
        { }
        virtual void LoadModelViewMatrix(const math::Matrix& modelView)
        { }
        virtual void SetMorphGrade(float grade)
        { }
        virtual unsigned int GetPositionLocation() const
        {
            return 0;
        }
        virtual unsigned int GetMorphPositionLocation() const
        {
            return 0;
        }
        virtual unsigned int GetColorLocation() const
        {
            return 0;
        }
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

        virtual std::shared_ptr<mono::IMorphingShader> CreateMorphingShader() const
        {
            return std::make_shared<NullMorphShader>();
        }
    };
}

TEST(EngineTest, Basic)
{
    mono::EventHandler handler;
    mono::LoadCustomShaderFactory(new NullFactory);

    std::shared_ptr<MocWindow> window = std::make_shared<MocWindow>(handler);
    std::shared_ptr<MocCamera> camera = std::make_shared<MocCamera>();
    std::shared_ptr<MocZone> zone = std::make_shared<MocZone>();
    
    {
        mono::Engine engine(window, camera, handler);
        EXPECT_NO_THROW(engine.Run(zone));
    }

    EXPECT_TRUE(window->mMakeCurrentCalled);
    EXPECT_TRUE(window->mSwapBuffersCalled);

    EXPECT_TRUE(camera->mUpdateCalled);
    EXPECT_TRUE(camera->mUnfollowCalled);
    
    EXPECT_TRUE(zone->mAcceptCalled);
    EXPECT_TRUE(zone->mOnLoadCalled);
    EXPECT_TRUE(zone->mOnUnloadCalled);

    mono::UnloadShaderFactory();
}

