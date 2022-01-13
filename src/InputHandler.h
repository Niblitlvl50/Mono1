
#pragma once

#include "MonoFwd.h"
#include "System/System.h"
#include <functional>

namespace mono
{
    using ScreenToWorldFunc = std::function<void (float& x, float& y)>;

    class InputHandler : public System::IInputHandler
    {
    public:

        InputHandler(const ScreenToWorldFunc& screen_to_world_func, EventHandler* event_handler);

    private:

        void OnKeyDown(Keycode key, bool ctrl, bool shift, bool alt, bool super) override;
        void OnKeyUp(Keycode key, bool ctrl, bool shift, bool alt, bool super) override;
        void OnTextInput(const char* text) override;
        void OnMouseDown(MouseButton button, int x, int y, bool ctrl, bool shift, bool alt, bool super) override;
        void OnMouseUp(MouseButton button, int x, int y, bool ctrl, bool shift, bool alt, bool super) override;
        void OnMouseMotion(int x, int y, bool ctrl, bool shift, bool alt, bool super) override;
        void OnMouseWheel(int x, int y, bool ctrl, bool shift, bool alt, bool super) override;
        void OnTouchDown(int64_t touchId, float x, float y, float dx, float dy) override;
        void OnTouchUp(int64_t touchId, float x, float y, float dx, float dy) override;
        void OnTouchMotion(int64_t touchId, float x, float y, float dx, float dy) override;
        void OnMultiGesture(float x, float y, float theta, float distance) override;

        void OnControllerAdded(int controller_id) override;
        void OnControllerRemoved(int controller_id) override;
        void OnControllerButtonDown(int controller_id, System::ControllerButton button) override;

        void OnAppTerminating() override;
        void OnEnterBackground() override;
        void OnEnterForeground() override;
        void OnQuit() override;

        void OnSurfaceChanged(int width, int height) override;
        void OnActivated(bool gain) override;

        const ScreenToWorldFunc m_screen_to_world_func;
        EventHandler* m_event_handler;
    };
}
