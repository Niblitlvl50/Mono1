//
//  main.cpp
//  AnimationTool
//
//  Created by Niklas Damberg on 28/11/15.
//
//

#include "SysLibs.h"
#include "SysVideo.h"
#include "SysKeycodes.h"
#include "WindowFactory.h"
#include "TraceCamera.h"
#include "Engine.h"
#include "EventHandler.h"
#include "KeyUpEvent.h"
#include "KeyDownEvent.h"
#include "EventFuncFwd.h"

#include "IWindow.h"
#include "ICamera.h"
#include "IZone.h"
#include "ZoneBase.h"
#include "Engine.h"
#include "Color.h"
#include "Sprite.h"
#include "EntityBase.h"
#include "IRenderer.h"
#include "TextFunctions.h"

#include <algorithm>
#include <cstdio>

namespace
{
    class Label : public mono::IDrawable
    {
    public:

        Label(const char* text, const math::Vector2f& position)
            : m_text(text),
              m_position(position),
              m_size(mono::MeasureString(text))
        { }

        virtual void doDraw(mono::IRenderer& renderer) const
        {
            constexpr mono::Color::RGBA color(1, 0, 0, 1);
            renderer.DrawText(m_text, m_position, false, color);
        }

        virtual math::Quad BoundingBox() const
        {
            math::Quad bb;
            bb.mA = m_position;
            bb.mB = m_position + m_size;

            return bb;
        }

        const char* m_text;
        const math::Vector2f m_position;
        const math::Vector2f m_size;
    };

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
    
    class Zone : public mono::ZoneBase
    {
    public:

        Zone(mono::EventHandler& eventHandler, const char* file)
            : m_eventHandler(eventHandler)
        {
            using namespace std::placeholders;
            const event::KeyDownEventFunc func = std::bind(&Zone::OnDownUp, this, _1);
            m_keyDownToken = eventHandler.AddListener(func);

            m_sprite = std::make_shared<SpriteDrawable>(file);
        }

        virtual void OnLoad(mono::ICameraPtr camera)
        {
            AddDrawable(std::make_shared<Label>("hello", math::Vector2f(50, 50)), 0);

            AddEntity(m_sprite, 0);
            camera->Follow(m_sprite, math::zeroVec);
            camera->SetPosition(m_sprite->Position());
        }

        virtual void OnUnload()
        { }

        void OnDownUp(const event::KeyDownEvent& event)
        {
            if(event.key == Key::LEFT)
            {
                m_sprite->PreviousAnimation();
                return;
            }
            else if(event.key == Key::RIGHT)
            {
                m_sprite->NextAnimation();
                return;
            }

            int animation = -1;

            if(event.key == Key::ZERO)
                animation = 0;
            else if(event.key == Key::ONE)
                animation = 1;
            else if(event.key == Key::TWO)
                animation = 2;
            else if(event.key == Key::THREE)
                animation = 3;
            else if(event.key == Key::FOUR)
                animation = 4;
            else if(event.key == Key::FIVE)
                animation = 5;
            else if(event.key == Key::SIX)
                animation = 6;
            else if(event.key == Key::SEVEN)
                animation = 7;
            else if(event.key == Key::EIGHT)
                animation = 8;
            else if(event.key == Key::NINE)
                animation = 9;

            m_sprite->SetAnimation(animation);
        }

        mono::EventHandler& m_eventHandler;
        mono::EventToken<event::KeyDownEvent> m_keyDownToken;
        std::shared_ptr<SpriteDrawable> m_sprite;
    };
}

int main(int argc, const char * argv[])
{
    if(argc < 2)
    {
        std::printf("You need to supply an argument");
        return 1;
    }

    // This is assumed to be the file argument
    const char* file = argv[1];

    System::Init();

    // The "global" event handler used throughout the game
    mono::EventHandler eventHandler;

    auto window = mono::CreateWindow("AnimationTool", 800, 600, false);
    window->SetBackgroundColor(mono::Color::RGBA(0.6, 0.6, 0.6));

    mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(300, 200, eventHandler);
    mono::LoadFont("pixelette.ttf", 10.0f);

    mono::Engine engine(window, camera, eventHandler);
    engine.Run(std::make_shared<Zone>(eventHandler, file));

    System::Exit();
    
    return 0;
}
