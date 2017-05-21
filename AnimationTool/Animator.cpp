
#include "Animator.h"
#include "MutableSprite.h"
#include "SpriteTexture.h"

#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/MultiGestureEvent.h"
#include "Events/SurfaceChangedEvent.h"

#include "Rendering/ICamera.h"

#include "Math/Vector.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"
#include "System/Keycodes.h"

#include "InterfaceDrawer.h"

#include "Rendering/Sprite/ISprite.h"
#include "Rendering/Sprite/SpriteFactory.h"
#include "Rendering/Sprite/AnimationSequence.h"

#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/TextureFactory.h"
#include "WriteSpriteFile.h"

using namespace animator;

namespace
{
    void SetupIcons(UIContext& context, std::unordered_map<unsigned int, mono::ITexturePtr>& textures)
    {
        mono::ITexturePtr texture = mono::CreateTexture("textures/animator_tools.png"); 
        textures.insert(std::make_pair(texture->Id(), texture));

        const mono::ISpritePtr save = mono::CreateSprite("sprites/save.sprite");
        const mono::ISpritePtr add = mono::CreateSprite("sprites/add.sprite");
        const mono::ISpritePtr plus = mono::CreateSprite("sprites/plus.sprite");
        const mono::ISpritePtr remove = mono::CreateSprite("sprites/delete.sprite");

        context.tools_texture_id = texture->Id();
        context.save_icon = save->GetTextureCoords();
        context.add_icon = add->GetTextureCoords();
        context.plus_icon = plus->GetTextureCoords();
        context.delete_icon = remove->GetTextureCoords();
    }
}

Animator::Animator(System::IWindow* window, mono::EventHandler& eventHandler, const char* sprite_file)
    : m_eventHandler(eventHandler),
      m_spriteFile(sprite_file),
      m_inputHandler(eventHandler)
{
    using namespace std::placeholders;
    
    const event::KeyDownEventFunc& func = std::bind(&Animator::OnDownUp, this, _1);
    const event::SurfaceChangedEventFunc& surface_func = std::bind(&Animator::OnSurfaceChanged, this, _1);
    const event::MouseWheelEventFunc& mouse_wheel = std::bind(&Animator::OnMouseWheel, this, _1);
    const event::MultiGestureEventFunc& multi_gesture = std::bind(&Animator::OnMultiGesture, this, _1);

    m_keyDownToken = eventHandler.AddListener(func);
    m_surfaceChangedToken = eventHandler.AddListener(surface_func);
    m_mouseWheelToken = m_eventHandler.AddListener(mouse_wheel);
    m_multiGestureToken = m_eventHandler.AddListener(multi_gesture);

    std::unordered_map<unsigned int, mono::ITexturePtr> textures;
    SetupIcons(m_context, textures);

    const System::Size& size = window->Size();
    const math::Vector window_size(size.width, size.height);
    m_guiRenderer = std::make_shared<ImGuiRenderer>("animator_imgui.ini", window_size, textures);

    mono::CreateSprite(m_sprite, sprite_file);

    m_sprite_texture = std::make_shared<SpriteTexture>(m_sprite.GetTexture(), m_sprite.GetFullTexureCoords());
    
    AddEntity(std::make_shared<MutableSprite>(m_sprite), 0);
    AddEntity(m_sprite_texture, 1);
    AddDrawable(m_guiRenderer, 2);
    AddUpdatable(std::make_shared<InterfaceDrawer>(m_context));

    // Setup UI callbacks
    m_context.on_loop_toggle      = std::bind(&Animator::OnLoopToggle, this, _1);
    m_context.on_add_animation    = std::bind(&Animator::OnAddAnimation, this);
    m_context.on_delete_animation = std::bind(&Animator::OnDeleteAnimation, this);
    m_context.on_add_frame        = std::bind(&Animator::OnAddFrame, this);
    m_context.on_delete_frame     = std::bind(&Animator::OnDeleteFrame, this, _1);
    m_context.on_name_animation   = std::bind(&Animator::OnNameAnimation, this, _1);
    m_context.on_set_animation    = std::bind(&Animator::SetAnimation, this, _1);
    m_context.on_save             = std::bind(&Animator::SaveSprite, this);

    m_context.max_frame_id = m_sprite.GetUniqueFrames() -1;
    m_context.active_frame = m_sprite.GetActiveAnimation();

    SetAnimation(m_sprite.GetActiveAnimation());
}

Animator::~Animator()
{
    m_eventHandler.RemoveListener(m_keyDownToken);
    m_eventHandler.RemoveListener(m_surfaceChangedToken);
    m_eventHandler.RemoveListener(m_mouseWheelToken);
    m_eventHandler.RemoveListener(m_multiGestureToken);
}

void Animator::OnLoad(mono::ICameraPtr camera)
{
    m_camera = camera;
    camera->SetPosition(math::zeroVec);
}

void Animator::OnUnload()
{
    m_camera = nullptr;
    SaveSprite();
}

void Animator::SetAnimation(int animation_id)
{
    const int animations = m_sprite.GetDefinedAnimations();
    if(animation_id < animations)
    {
        m_sprite.SetAnimation(animation_id);
        UpdateUIContext(animation_id);
    }
}

void Animator::UpdateUIContext(int animation_id)
{
    mono::AnimationSequence& sequence = m_sprite.GetSequence(animation_id);

    m_context.animations = m_sprite.GetDefinedAnimations();
    m_context.animation_id = animation_id;
    m_context.display_name = sequence.GetName();
    m_context.loop_animation = sequence.IsLooping();
    m_context.frames = &sequence.GetFrames();
}

bool Animator::OnDownUp(const event::KeyDownEvent& event)
{
    int animation = -1;
    
    switch(event.key)
    {
        case Keycode::ENTER:
        case Keycode::SPACE:
        {
            m_sprite.RestartAnimation();
            return true;
        }
        case Keycode::TAB:
            m_sprite_texture->Enable(!m_sprite_texture->Enabled());
            return true;
        case Keycode::LEFT:
        case Keycode::DOWN:
        {
            int id = m_sprite.GetActiveAnimation();
            --id;
            animation = std::max(id, 0);
            break;
        }
        case Keycode::RIGHT:
        case Keycode::UP:
        {
            int id = m_sprite.GetActiveAnimation();
            ++id;
            animation = std::min(id, m_sprite.GetDefinedAnimations() -1);
            break;
        }
        case Keycode::ZERO:
            animation = 0;
            break;
        case Keycode::ONE:
            animation = 1;
            break;
        case Keycode::TWO:
            animation = 2;
            break;
        case Keycode::THREE:
            animation = 3;
            break;
        case Keycode::FOUR:
            animation = 4;
            break;
        case Keycode::FIVE:
            animation = 5;
            break;
        case Keycode::SIX:
            animation = 6;
            break;
        case Keycode::SEVEN:
            animation = 7;
            break;
        case Keycode::EIGHT:
            animation = 8;
            break;
        case Keycode::NINE:
            animation = 9;
            break;
        default:
            break;
    }

    if(animation != -1)
    {
        SetAnimation(animation);
        return true;
    }

    return false;
}

bool Animator::OnMouseWheel(const event::MouseWheelEvent& event)
{
    const float multiplier = (event.y < 0.0f) ? 1.0f : -1.0f;
    Zoom(multiplier);
    return true;
}

bool Animator::OnMultiGesture(const event::MultiGestureEvent& event)
{
    if(std::fabs(event.distance) < 1e-3)
        return false;

    const float multiplier = (event.distance < 0.0f) ? 1.0f : -1.0f;
    Zoom(multiplier);
    return true;
}

bool Animator::OnSurfaceChanged(const event::SurfaceChangedEvent& event)
{
    if(event.width > 0 && event.height > 0)
        m_guiRenderer->SetWindowSize(math::Vector(event.width, event.height));

    return false;
}

void Animator::OnLoopToggle(bool state)
{
    const int current_id = m_sprite.GetActiveAnimation();
    mono::AnimationSequence& sequence = m_sprite.GetSequence(current_id);
    sequence.SetLooping(state);

    m_sprite.RestartAnimation();
}

void Animator::OnAddAnimation()
{
    const int animation_id = m_sprite.DefineAnimation("new", { 0, 100 }, false);
    SetAnimation(animation_id);
    m_sprite.RestartAnimation();
}

void Animator::OnDeleteAnimation()
{
    const int active_animation = m_sprite.GetActiveAnimation();
    
    std::vector<mono::AnimationSequence>& animations = m_sprite.GetAnimations();
    animations.erase(animations.begin() + active_animation);

    if(active_animation >= static_cast<int>(animations.size()))
        SetAnimation(active_animation -1);
}

void Animator::OnAddFrame()
{
    const int current_id = m_sprite.GetActiveAnimation();
    m_sprite.GetSequence(current_id).AddFrame(0, 100);
}

void Animator::OnDeleteFrame(int id)
{
    const int current_id = m_sprite.GetActiveAnimation();
    m_sprite.GetSequence(current_id).RemoveFrame(id);
}

void Animator::OnNameAnimation(const char* new_name)
{
    const int current_id = m_sprite.GetActiveAnimation();
    m_sprite.GetSequence(current_id).SetName(new_name);
}

void Animator::Zoom(float multiplier)
{
    math::Quad quad = m_camera->GetViewport();

    const float resizeValue = quad.mB.x * 0.15f * multiplier;
    const float aspect = quad.mB.x / quad.mB.y;
    math::ResizeQuad(quad, resizeValue, aspect);

    m_camera->SetTargetViewport(quad);
}

void Animator::SaveSprite()
{
    WriteSpriteFile(m_spriteFile, m_sprite.GetAnimations());
}

