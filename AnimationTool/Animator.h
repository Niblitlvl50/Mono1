
#pragma once

#include "System/System.h"
#include "MonoFwd.h"
#include "Zone/ZoneBase.h"
#include "EventHandler/EventToken.h"
#include "Events/EventFwd.h"
#include "UIContext.h"
#include "Sprite/Sprite.h"

#include "ImGuiImpl/ImGuiInputHandler.h"
#include "ImGuiImpl/ImGuiRenderer.h"

#include <memory>

namespace animator
{
    class MutableSprite;

    class Animator : public mono::ZoneBase
    {
    public:

        Animator(System2::IWindow* window, mono::EventHandler& eventHandler, const char* sprite_file);
        ~Animator();

        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();

        void SetAnimation(int animation_id);
        void UpdateUIContext(int animation_id);

        bool OnDownUp(const event::KeyDownEvent& event);
        bool OnMouseWheel(const event::MouseWheelEvent& event);
        bool OnMultiGesture(const event::MultiGestureEvent& event);
        bool OnSurfaceChanged(const event::SurfaceChangedEvent& event);
        
        void OnLoopToggle(bool state);
        void OnAddAnimation();
        void OnDeleteAnimation();
        void OnAddFrame();
        void OnDeleteFrame(int id);
        void OnNameAnimation(const char* new_name);

        void Zoom(float multiplier);
        void SaveSprite();

    private:

        mono::EventHandler& m_eventHandler;
        const char* m_spriteFile;

        mono::EventToken<event::KeyDownEvent> m_keyDownToken;
        mono::EventToken<event::SurfaceChangedEvent> m_surfaceChangedToken;
        mono::EventToken<event::MouseWheelEvent> m_mouseWheelToken;
        mono::EventToken<event::MultiGestureEvent> m_multiGestureToken;
        
        std::shared_ptr<ImGuiRenderer> m_guiRenderer;

        ImGuiInputHandler m_inputHandler;
        UIContext m_context;

        mono::ICameraPtr m_camera;
        mono::Sprite m_sprite;
    };
}