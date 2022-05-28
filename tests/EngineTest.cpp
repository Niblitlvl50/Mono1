
#include "gtest/gtest.h"

#include "System/System.h"

#include "Engine.h"
#include "SystemContext.h"
#include "MonoFwd.h"
#include "IUpdatable.h"
#include "Camera/Camera.h"
#include "Zone/IZone.h"
#include "Rendering/RenderSystem.h"

#include "Rendering/Texture/ITexture.h"
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
            : m_event_handler(handler)
        {
            m_position.x = 0;
            m_position.y = 0;
            m_size.width = 640;
            m_size.height = 480;
        }
        void Maximize() override
        { }
        void Minimize() override
        { }
        void RestoreSize() override
        { }
        void SwapBuffers() const override
        {
            m_swap_buffers_called = true;
            m_event_handler.DispatchEvent(event::QuitEvent());
        }
        void MakeCurrent() override
        {
            m_make_current_called = true;
        }
        System::Position Position() const override
        {
            return m_position;
        }
        System::Size Size() const override
        {
            return m_size;
        }
        System::Size DrawableSize() const override
        {
            return m_size;
        }
        System::WindowOptions Options() const override
        {
            return System::WindowOptions(0);
        }

        mono::EventHandler& m_event_handler;
        System::Position m_position;
        System::Size m_size;

        bool m_make_current_called = false;
        mutable bool m_swap_buffers_called = false;
    };

    struct MocZone : mono::IZone
    {
        MocZone()
        { }
        void Accept(mono::IRenderer& renderer) override
        {
            m_accept_called = true;
        }
        void Accept(mono::IUpdater& updater) override
        { }
        void OnLoad(mono::ICamera* camera, mono::IRenderer* renderer) override
        {
            m_on_load_called = true;
        }
        int OnUnload() override
        {
            m_on_unload_called = true;
            return 0;
        }
        void PostUpdate() override
        { }
        void AddDrawable(mono::IDrawable* drawable, int layer) override
        { }
        void RemoveDrawable(mono::IDrawable* drawable) override
        { }
        void AddUpdatable(mono::IUpdatable* updatable) override
        { }
        void RemoveUpdatable(mono::IUpdatable* updatable) override
        { }
        void SetDrawableLayer(const mono::IDrawable* drawable, int new_layer) override
        { }
        void SetLastLightingLayer(int layer) override
        { }

        bool m_accept_called = false;
        bool m_on_load_called = false;
        bool m_on_unload_called = false;
    };

    class NullTexture : public mono::ITexture
    {
    public:

        uint32_t Id() const override
        {
            return 0;
        }
        uint32_t Width() const override
        {
            return 16;
        }
        uint32_t Height() const override
        {
            return 9;
        }
    };

    class NullTextureFactory : public mono::ITextureFactory
    {
    public:

        mono::ITexturePtr CreateTexture(const char* texture_name) const
        {
            return std::make_shared<NullTexture>();
        }

        mono::ITexturePtr CreateTextureFromData(const byte* data, int data_length, const char* cache_name) const
        {
            return std::make_shared<NullTexture>();
        }

        mono::ITexturePtr CreateTexture(const byte* data, int width, int height, int color_components) const
        {
            return std::make_shared<NullTexture>();
        }

        mono::ITexturePtr CreateFromNativeHandle(uint32_t native_handle) const
        {
            return std::make_shared<NullTexture>();
        }
    };
}

TEST(EngineTest, DISABLED_Basic)
{
    mono::EventHandler handler;
    mono::SystemContext system_context;
    mono::Camera camera;
    mono::LoadCustomTextureFactory(new NullTextureFactory);

    MocWindow window(handler);
    MocZone zone;

    {
        mono::Engine engine(&window, &camera, &system_context, &handler);
        EXPECT_NO_THROW(engine.Run(&zone));
    }

    EXPECT_TRUE(window.m_make_current_called);
    EXPECT_TRUE(window.m_swap_buffers_called);

    EXPECT_TRUE(zone.m_accept_called);
    EXPECT_TRUE(zone.m_on_load_called);
    EXPECT_TRUE(zone.m_on_unload_called);
}

