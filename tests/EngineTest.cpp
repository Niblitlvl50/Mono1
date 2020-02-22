
#include "gtest/gtest.h"

#include "System/System.h"

#include "Engine.h"
#include "SystemContext.h"
#include "MonoFwd.h"
#include "IUpdatable.h"
#include "Camera/ICamera.h"
#include "Zone/IZone.h"
#include "Rendering/Shader/IShader.h"
#include "Rendering/Shader/IShaderFactory.h"
#include "Rendering/Shader/ShaderFunctions.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/Texture/ITextureFactory.h"

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
        virtual void doUpdate(const mono::UpdateContext& update_context) override
        {
            mUpdateCalled = true;
        }
        virtual void Follow(uint32_t entity_id, const math::Vector& offset) override
        { }
        virtual void Unfollow() override
        {
            mUnfollowCalled = true;
        }
        virtual math::Quad GetViewport() const override
        {
            return mViewport;
        }
        virtual math::Vector GetPosition() const override
        {
            return mViewport.mA + (mViewport.mB * 0.5f);
        }
        virtual void SetViewport(const math::Quad& viewport) override
        { }
        virtual void SetTargetViewport(const math::Quad& target) override
        { }
        virtual void SetPosition(const math::Vector& position) override
        { }
        math::Vector ScreenToWorld(const math::Vector& screen_pos, const math::Vector& window_size) const override
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
        virtual void Accept(mono::IRenderer& renderer) override
        {
            mAcceptCalled = true;
        }
        virtual void Accept(mono::IUpdater& updater) override
        { }
        virtual void OnLoad(mono::ICameraPtr& camera) override
        {
            mOnLoadCalled = true;
        }
        virtual int OnUnload() override
        {
            mOnUnloadCalled = true;
            return 0;
        }
        virtual void AddEntity(const mono::IEntityPtr& entity, int layer) override
        { }
        virtual void RemoveEntity(const mono::IEntityPtr& entity) override
        { }
        virtual void AddDrawable(const mono::IDrawablePtr& drawable, int layer) override
        { }
        virtual void RemoveDrawable(const mono::IDrawablePtr& drawable) override
        { }
        virtual void AddUpdatable(const mono::IUpdatablePtr& updatable) override
        { }
        virtual void RemoveUpdatable(const mono::IUpdatablePtr& updatable) override
        { }
        virtual void SetDrawableLayer(const mono::IDrawablePtr& drawable, int new_layer) override
        { }
        virtual mono::IEntityPtr FindEntityFromId(uint32_t id) const override
        {
            return nullptr;
        }

        bool mAcceptCalled = false;
        bool mOnLoadCalled = false;
        bool mOnUnloadCalled = false;
    };

    class NullShader : public mono::IShader
    {
    public:
        
        void Use() override
        { }
        uint32_t GetShaderId() const override
        {
            return 0;
        }
        int GetAttributeLocation(const char* attribute_name) override
        {
            return 0;
        }
        void SetValue(const char* property_name, int value) override
        {}
        void SetValue(const char* property_name, float value) override
        {}
        void SetValue(const char* property_name, const math::Vector& vector) override
        {}
        void SetValue(const char* property_name, const math::Matrix& transform) override
        {}
        void SetValue(const char* property_name, const mono::Color::RGBA& color) override
        {}
        void SetProjectionAndModelView(const math::Matrix& projection, const math::Matrix& model_view) override
        {}
    };

    class NullShaderFactory : public mono::IShaderFactory
    {
    public:
        std::unique_ptr<mono::IShader> CreateTextureShader() const override
        {
            return std::make_unique<NullShader>();
        }
        std::unique_ptr<mono::IShader> CreateColorShader() const override
        {
            return std::make_unique<NullShader>();
        }
        std::unique_ptr<mono::IShader> CreatePointSpriteShader() const override
        {
            return std::make_unique<NullShader>();
        }
        std::unique_ptr<mono::IShader> CreateImGuiShader() const override
        {
            return std::make_unique<NullShader>();
        }
    };

    class NullTextureFactory : public mono::ITextureFactory
    {
    public:

        mono::ITexturePtr CreateTexture(const char* texture_name) const
        {
            return nullptr;
        }

        mono::ITexturePtr CreateTextureFromData(const byte* data, int data_length, const char* cache_name) const
        {
            return nullptr;
        }

        mono::ITexturePtr CreateTexture(const byte* data, int width, int height, int color_components) const
        {
            return nullptr;
        }
    };
}

TEST(EngineTest, Basic)
{
    mono::EventHandler handler;
    mono::SystemContext system_context;
    mono::LoadCustomShaderFactory(new NullShaderFactory);
    mono::LoadCustomTextureFactory(new NullTextureFactory);

    MocWindow window(handler);
    std::shared_ptr<MocCamera> camera = std::make_shared<MocCamera>();
    std::shared_ptr<MocZone> zone = std::make_shared<MocZone>();
    
    {
        mono::Engine engine(&window, camera, &system_context, &handler);
        EXPECT_NO_THROW(engine.Run(zone.get()));
    }

    EXPECT_TRUE(window.mMakeCurrentCalled);
    EXPECT_TRUE(window.mSwapBuffersCalled);

    EXPECT_TRUE(camera->mUpdateCalled);
    EXPECT_TRUE(camera->mUnfollowCalled);
    
    EXPECT_TRUE(zone->mAcceptCalled);
    EXPECT_TRUE(zone->mOnLoadCalled);
    EXPECT_TRUE(zone->mOnUnloadCalled);
}

