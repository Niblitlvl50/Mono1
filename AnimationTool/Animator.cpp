
#include "Animator.h"
#include "MutableSprite.h"

#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/KeyEvent.h"
#include "Events/SurfaceChangedEvent.h"

#include "ICamera.h"
#include "IWindow.h"

#include "Math/Vector2f.h"
#include "System/SysKeycodes.h"

#include "InterfaceDrawer.h"

#include "Sprite/AnimationSequence.h"
#include "Texture/ITexture.h"
#include "Texture/TextureFactory.h"
#include "WriteSpriteFile.h"

using namespace animator;

Animator::Animator(const mono::IWindowPtr& window, mono::EventHandler& eventHandler, const char* sprite_file)
    : m_eventHandler(eventHandler),
      m_spriteFile(sprite_file),
      m_inputHandler(eventHandler)
{
    using namespace std::placeholders;
    const event::KeyDownEventFunc func = std::bind(&Animator::OnDownUp, this, _1);
    m_keyDownToken = eventHandler.AddListener(func);

    const event::SurfaceChangedEventFunc surface_func = std::bind(&Animator::OnSurfaceChanged, this, _1);
    m_surfaceChangedToken = eventHandler.AddListener(surface_func);

    mono::ITexturePtr texture = mono::CreateTexture("placeholder.png");
    std::unordered_map<unsigned int, mono::ITexturePtr> textures;
    textures.insert(std::make_pair(texture->Id(), texture));

    // Setup UI callbacks
    m_context.on_loop_toggle = std::bind(&Animator::OnLoopToggle, this, _1);
    m_context.on_tool_callback = std::bind(&Animator::OnToolAction, this, _1);

    // Setup texture id's
    m_context.save_icon = texture->Id();
    m_context.zoom_in_icon = texture->Id();
    m_context.zoom_out_icon = texture->Id();

    m_sprite = std::make_shared<MutableSprite>(sprite_file);
    m_guiRenderer = std::make_shared<ImGuiRenderer>("animator_imgui.ini", window->Size(), textures);
    
    AddEntity(m_sprite, 0);
    AddDrawable(m_guiRenderer, 2);
    AddUpdatable(std::make_shared<InterfaceDrawer>(m_context));

    SetAnimation(0);
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
{
    WriteSpriteFile(m_spriteFile, m_sprite->GetAnimations());
}

void Animator::SetAnimation(int animation_id)
{
    m_sprite->SetAnimation(animation_id);
    UpdateUIContext(animation_id);
}

void Animator::UpdateUIContext(int animation_id)
{
    mono::AnimationSequence& sequence = m_sprite->GetSequence(animation_id);

    m_context.animation_id = animation_id;
    m_context.display_name = "hello";
    m_context.loop_animation = sequence.IsLooping();
    m_context.frames = &sequence.GetFrames();
}

bool Animator::OnDownUp(const event::KeyDownEvent& event)
{
    int animation = -1;
    
    if(event.key == Key::ENTER || event.key == Key::SPACE)
    {
        m_sprite->RestartAnimation();
        return true;
    }
    else if(event.key == Key::LEFT || event.key == Key::DOWN)
    {
        animation = m_sprite->PreviousAnimation();
    }
    else if(event.key == Key::RIGHT || event.key == Key::UP)
    {
        animation = m_sprite->NextAnimation();
    }
    else
    {
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
    }

    if(animation != -1)
    {
        SetAnimation(animation);
        return true;
    }

    return false;
}

bool Animator::OnSurfaceChanged(const event::SurfaceChangedEvent& event)
{
    if(event.width > 0 && event.height > 0)
        m_guiRenderer->SetWindowSize(math::Vector2f(event.width, event.height));

    return false;
}

void Animator::OnLoopToggle(bool state)
{
    const int current_id = m_sprite->CurrentAnimation();
    mono::AnimationSequence& sequence = m_sprite->GetSequence(current_id);
    sequence.SetLooping(state);

    m_sprite->RestartAnimation();
}

void Animator::OnToolAction(Action tool_action)
{
    switch(tool_action)
    {
    case Action::ZOOM_IN:
        m_sprite->SetScale(m_sprite->Scale() * 1.1f);
        break;

    case Action::ZOOM_OUT:
        m_sprite->SetScale(m_sprite->Scale() * 0.9f);
        break;

    case Action::SAVE:
        WriteSpriteFile(m_spriteFile, m_sprite->GetAnimations());
        break;
    }
}
