
#pragma once

#include "MonoFwd.h"
#include "System/System.h"
#include <functional>

namespace mono
{
    using ScreenToWorldFunc = std::function<void (float&, float&)>;

    class InputHandler : public System::IInputHandler
    {
    public:

        InputHandler(const ScreenToWorldFunc& screen_to_world_func, EventHandler* event_handler);

    private:

        virtual void OnKeyDown(Keycode key, bool ctrl, bool shift, bool alt);
        virtual void OnKeyUp(Keycode key, bool ctrl, bool shift, bool alt);
        virtual void OnTextInput(const char* text);
        virtual void OnMouseDown(MouseButton button, int x, int y);
        virtual void OnMouseUp(MouseButton button, int x, int y);
        virtual void OnMouseMotion(int x, int y);
        virtual void OnMouseWheel(int x, int y);
        virtual void OnTouchDown(int64_t touchId, float x, float y, float dx, float dy);
        virtual void OnTouchUp(int64_t touchId, float x, float y, float dx, float dy);
        virtual void OnTouchMotion(int64_t touchId, float x, float y, float dx, float dy);
        virtual void OnMultiGesture(float x, float y, float theta, float distance);

        virtual void OnControllerAdded(int controller_id);
        virtual void OnControllerRemoved(int controller_id);

        virtual void OnAppTerminating();
        virtual void OnEnterBackground();
        virtual void OnEnterForeground();
        virtual void OnQuit();

        virtual void OnSurfaceChanged(int width, int height);
        virtual void OnActivated(bool gain);

        const ScreenToWorldFunc m_screen_to_world_func;
        EventHandler* m_event_handler;
    };
}
