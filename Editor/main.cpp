

#include "SysLibs.h"

#include "WindowFactory.h"
#include "TraceCamera.h"
#include "Engine.h"

#include "EventHandler.h"

#include "IWindow.h"
#include "ICamera.h"
#include "IZone.h"
#include "IRenderer.h"

#include "ZoneBase.h"
#include "TextFunctions.h"
#include "Color.h"
#include "Sprite.h"
#include "EntityBase.h"

#include "ImGuiInputHandler.h"
#include "ImGuiInterfaceDrawer.h"
#include "ImGuiRenderer.h"


namespace
{
    class SpriteDrawable : public mono::EntityBase
    {
    public:

        SpriteDrawable(const char* file)
        : m_sprite(file)
        {
            mPosition = math::Vector2f(100, 100);
            mScale = math::Vector2f(50, 50);
        }

        virtual void Draw(mono::IRenderer& renderer) const
        {
            renderer.DrawSprite(m_sprite);
        }

        virtual void Update(unsigned int delta)
        {
            m_sprite.doUpdate(delta);
        }

        void SetAnimation(int id)
        {
            const unsigned int animations = m_sprite.GetDefinedAnimations();
            if(id < animations)
                m_sprite.SetAnimation(id);
        }

        void NextAnimation()
        {
            int id = m_sprite.GetActiveAnimation();

            id++;

            id = std::min(id, m_sprite.GetDefinedAnimations() -1);
            m_sprite.SetAnimation(id);
        }

        void PreviousAnimation()
        {
            int id = m_sprite.GetActiveAnimation();
            id--;

            id = std::max(id, 0);
            m_sprite.SetAnimation(id);
        }
        
        mono::Sprite m_sprite;
    };

    class EditorZone : public mono::ZoneBase
    {
    public:

        EditorZone(const math::Vector2f& window_size, mono::EventHandler& event_handler)
            : m_windowSize(window_size),
              m_inputHandler(event_handler)
        { }

        virtual ~EditorZone()
        { }

        virtual void OnLoad(mono::ICameraPtr camera)
        {
            AddUpdatable(std::make_shared<editor::ImGuiInterfaceDrawer>());
            AddDrawable(std::make_shared<editor::ImGuiRenderer>(m_windowSize), 0);

            AddDrawable(std::make_shared<SpriteDrawable>("shuttle.sprite"), 0);
        }

        virtual void OnUnload()
        { }

        const math::Vector2f m_windowSize;
        editor::ImGuiInputHandler m_inputHandler;
    };
}

int main()
{
    constexpr math::Vector2f window_size(1200.0f, 800.0f);

    System::Init();

    // The "global" event handler used throughout the game
    mono::EventHandler eventHandler;

    mono::IWindowPtr window = mono::CreateWindow("Editor", window_size.x, window_size.y, false);
    window->SetBackgroundColor(mono::Color::RGBA(0.7, 0.7, 0.7));

    mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(window_size.x, window_size.y, eventHandler);
    //mono::LoadFont("pixelette.ttf", 10.0f);

    mono::Engine engine(window, camera, eventHandler);
    engine.Run(std::make_shared<EditorZone>(window_size, eventHandler));

    System::Exit();

    return 0;
}


