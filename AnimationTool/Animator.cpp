
#include "Animator.h"
#include "MutableSprite.h"

#include "EventHandler/EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/EventFuncFwd.h"

#include "ICamera.h"
#include "IWindow.h"

#include "Math/Vector2f.h"
#include "System/SysKeycodes.h"

#include "InterfaceDrawer.h"
#include "ImGuiImpl/ImGuiRenderer.h"


using namespace animator;

Animator::Animator(const mono::IWindowPtr& window, mono::EventHandler& eventHandler, const char* file)
    : m_eventHandler(eventHandler),
      m_inputHandler(eventHandler)
{
    using namespace std::placeholders;
    const event::KeyDownEventFunc func = std::bind(&Animator::OnDownUp, this, _1);
    m_keyDownToken = eventHandler.AddListener(func);

    m_sprite = std::make_shared<MutableSprite>(file);

    AddUpdatable(std::make_shared<InterfaceDrawer>());

    AddEntity(m_sprite, 0);
    AddDrawable(std::make_shared<ImGuiRenderer>("animator_imgui.ini", window->Size()), 2);
}

Animator::~Animator()
{
    m_eventHandler.RemoveListener(m_keyDownToken);
}

void Animator::OnLoad(mono::ICameraPtr camera)
{
    camera->Follow(m_sprite, math::zeroVec);
    camera->SetPosition(m_sprite->Position());
}

void Animator::OnUnload()
{ }

bool Animator::OnDownUp(const event::KeyDownEvent& event)
{
    if(event.key == Key::LEFT)
    {
        m_sprite->PreviousAnimation();
        return true;
    }
    else if(event.key == Key::RIGHT)
    {
        m_sprite->NextAnimation();
        return true;
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

    return true;
}