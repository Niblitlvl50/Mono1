
#include "gtest/gtest.h"

#include "System/System.h"

#include "Engine.h"
#include "SystemContext.h"
#include "MonoFwd.h"
#include "Camera/ICamera.h"
#include "Zone/IZone.h"
#include "Rendering/Shader/IShaderFactory.h"
#include "Rendering/Shader/IColorShader.h"
#include "Rendering/Shader/ITextureShader.h"
#include "Rendering/Shader/IPointSpriteShader.h"
#include "Rendering/Shader/ShaderFunctions.h"
#include "Rendering/RenderSystem.h"

#include "Math/Vector.h"
#include "Math/Quad.h"

#include "EventHandler/EventHandler.h"
#include "Events/QuitEvent.h"

namespace
{
    class MocWindow : public System::IWindow
    {
    public:
        MocWindow(mono::EventHandler& handler)
            : mHandler(handler)
        {
            m_position.x = 0;
            m_position.y = 0;
            m_size.width = 640;
            m_size.height = 480;
        }
        void SurfaceChanged(int width, int height) override
        { }
        void Maximize() override
        { }
        void Minimize() override
        { }
        void RestoreSize() override
        { }
        void SwapBuffers() const override
        {
            mSwapBuffersCalled = true;
            mHandler.DispatchEvent(event::QuitEvent());
        }
        void MakeCurrent() override
        {
            mMakeCurrentCalled = true;
        }
        void SetBackgroundColor(float red, float green, float blue) override
        { }
        const System::Position& Position() const override
        {
            return m_position;
        }
        const System::Size& Size() const override
        {
            return m_size;
        }

        mono::EventHandler& mHandler;
        System::Position m_position;
        System::Size m_size;

        bool mMakeCurrentCalled = false;
        mutable bool mSwapBuffersCalled = false;
    };

    struct MocCamera : mono::ICamera
    {
        MocCamera()
            : mViewport(0.0f, 0.0f, 100.0f, 50.0f)
        { }
        virtual void doUpdate(unsigned int delta)
        {
            mUpdateCalled = true;
        }
        virtual void Follow(uint32_t entity_id, const math::Vector& offset)
        { }
        virtual void Unfollow()
        {
            mUnfollowCalled = true;
        }
        virtual math::Quad GetViewport() const
        {
            return mViewport;
        }
        virtual math::Vector GetPosition() const
        {
            return mViewport.mA + (mViewport.mB * 0.5f);
        }
        virtual void SetViewport(const math::Quad& viewport)
        { }
        virtual void SetTargetViewport(const math::Quad& target)
        { }
        virtual void SetPosition(const math::Vector& position)
        { }
        math::Vector ScreenToWorld(const math::Vector& screen_pos, const math::Vector& window_size) const
        {
            return screen_pos;
        }

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
        virtual void Accept(mono::IUpdater& updater)
        { }
        virtual void OnLoad(mono::ICameraPtr& camera)
        {
            mOnLoadCalled = true;
        }
        virtual int OnUnload()
        {
            mOnUnloadCalled = true;
            return 0;
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
        virtual void SetDrawableLayer(const mono::IDrawablePtr& drawable, int new_layer)
        { }
        virtual mono::IEntityPtr FindEntityFromId(unsigned int id) const
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
        virtual void SetTextureOffset(float offset)
        { }
    };

    class NullPointSpriteShader : public mono::IPointSpriteShader
    {
    public:

        virtual void Use() override
        { }
        virtual unsigned int GetShaderId() const override
        {
            return 0;
        }
        virtual void LoadProjectionMatrix(const math::Matrix& projection) override
        { }
        virtual void LoadModelViewMatrix(const math::Matrix& modelView) override
        { }
        virtual unsigned int GetPositionAttribute() const override
        {
            return 0;
        }
        virtual unsigned int GetRotationAttribute() const override
        {
            return 0;
        }
        virtual unsigned int GetColorAttribute() const override
        {
            return 0;
        }
        virtual unsigned int GetPointSizeAttribute() const override
        {
            return 0;
        }
    };

    class NullFactory : public mono::IShaderFactory
    {
    public:
        virtual std::unique_ptr<mono::ITextureShader> CreateTextureShader() const
        {
            return std::make_unique<NullTextureShader>();
        }

        virtual std::unique_ptr<mono::IColorShader> CreateColorShader() const
        {
            return std::make_unique<NullColorShader>();
        }

        virtual std::unique_ptr<mono::IPointSpriteShader> CreatePointSpriteShader() const
        {
            return std::make_unique<NullPointSpriteShader>();
        }
    };
}

TEST(EngineTest, Basic)
{
    mono::EventHandler handler;
    mono::SystemContext system_context;
    mono::LoadCustomShaderFactory(new NullFactory);

    MocWindow window(handler);
    std::shared_ptr<MocCamera> camera = std::make_shared<MocCamera>();
    std::shared_ptr<MocZone> zone = std::make_shared<MocZone>();
    
    {
        mono::Engine engine(&window, camera, &system_context, handler);
        EXPECT_NO_THROW(engine.Run(zone));
    }

    EXPECT_TRUE(window.mMakeCurrentCalled);
    EXPECT_TRUE(window.mSwapBuffersCalled);

    EXPECT_TRUE(camera->mUpdateCalled);
    EXPECT_TRUE(camera->mUnfollowCalled);
    
    EXPECT_TRUE(zone->mAcceptCalled);
    EXPECT_TRUE(zone->mOnLoadCalled);
    EXPECT_TRUE(zone->mOnUnloadCalled);
}

