
#pragma once

#include "MonoFwd.h"
#include "ZoneBase.h"
#include "EventHandler/EventToken.h"
#include "Events/EventFwd.h"

#include "ImGuiImpl/ImGuiInputHandler.h"

#include <memory>

namespace animator
{
    class MutableSprite;

    class Animator : public mono::ZoneBase
    {
    public:

        Animator(const mono::IWindowPtr& window, mono::EventHandler& eventHandler, const char* file);
        ~Animator();

        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();

        bool OnDownUp(const event::KeyDownEvent& event);

    private:

        mono::EventHandler& m_eventHandler;
        mono::EventToken<event::KeyDownEvent> m_keyDownToken;
        std::shared_ptr<MutableSprite> m_sprite;

        ImGuiInputHandler m_inputHandler;
    };
}