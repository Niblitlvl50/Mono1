
#pragma once

#include "MonoFwd.h"
#include "ZoneBase.h"
#include "EventHandler/EventToken.h"
#include "Events/EventFwd.h"
#include "UIContext.h"

#include "ImGuiImpl/ImGuiInputHandler.h"
#include "ImGuiImpl/ImGuiRenderer.h"

#include <memory>

namespace animator
{
    class MutableSprite;

    class Animator : public mono::ZoneBase
    {
    public:

        Animator(const mono::IWindowPtr& window, mono::EventHandler& eventHandler, const char* sprite_file);
        ~Animator();

        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();

        void SetAnimation(int animation_id);
        void UpdateUIContext(int animation_id);

        bool OnDownUp(const event::KeyDownEvent& event);
        bool OnSurfaceChanged(const event::SurfaceChangedEvent& event);
        
        void OnLoopToggle(bool state);
        
        void OnAddAnimation();
        void OnDeleteAnimation(int id);
        void OnAddFrame();
        void OnDeleteFrame(int id);

        void OnToolAction(Action tool_action);

    private:

        mono::EventHandler& m_eventHandler;
        const char* m_spriteFile;

        mono::EventToken<event::KeyDownEvent> m_keyDownToken;
        mono::EventToken<event::SurfaceChangedEvent> m_surfaceChangedToken;
        std::shared_ptr<MutableSprite> m_sprite;
        std::shared_ptr<ImGuiRenderer> m_guiRenderer;

        ImGuiInputHandler m_inputHandler;
        UIContext m_context;
    };
}